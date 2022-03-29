/*
 * "Crescendo" firmware (ramping UI for clicky-switch lights)
 *
 * Copyright (C) 2017 Selene Scriven
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *
 * ATTINY13 Diagram
 *           ----
 *         -|1  8|- VCC
 *         -|2  7|- Voltage ADC
 *         -|3  6|-
 *     GND -|4  5|- PWM (Nx7135)
 *           ----
 *
 * FUSES
 *      (check bin/flash*.sh for recommended values)
 *
 * CALIBRATION
 *
 *   To find out what values to use, flash the driver with battcheck.hex
 *   and hook the light up to each voltage you need a value for.  This is
 *   much more reliable than attempting to calculate the values from a
 *   theoretical formula.
 *
 *   Same for off-time capacitor values.  Measure, don't guess.
 */

// User-configurable options
#include "config-default.h"

// Ramp config
#include "ramp-config.h"

// Also, assign I/O pins in this file:
#include "tk-attiny.h"

// Calibrate voltage and OTC in this file:
#include "tk-calibration.h"

/*
 * =========================================================================
 */

// Ignore a spurious warning, we did the cast on purpose
#pragma GCC diagnostic ignored "-Wint-to-pointer-cast"

#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <avr/sleep.h>
#include <string.h>

#define OWN_DELAY           // Don't use stock delay functions.
#define USE_DELAY_4MS
#ifdef PARTY_STROBES
#define USE_DELAY_MS
#define USE_FINE_DELAY
#endif
#define USE_DELAY_S         // Also use _delay_s(), not just _delay_ms()
#include "tk-delay.h"

#ifdef THERMAL_REGULATION
#define TEMP_10bit
#endif
#include "tk-voltage.h"

#ifdef RANDOM_STROBE
#include "tk-random.h"
#endif

#include "crescendo.h"

#include "misc.h"
#include "load-save.h"
#include "ramp-mode.h"
#include "battcheck.h"
#include "thermal-regulation.h"
#include "config-mode.h"
#include "goodnight-mode.h"
#include "strobes.h"

/********* Include all the app logic source files *********/
// (is a bit weird to do things this way,
//  but it saves a lot of space by letting us use the -fwhole-program flag)

#include "misc.c"
#include "load-save.c"
#include "ramp-mode.c"
#include "battcheck.c"
#include "thermal-regulation.c"
#include "config-mode.c"
#include "goodnight-mode.c"
#include "strobes.c"


inline void hw_setup() {
    // Set PWM pin to output
    DDRB |= (1 << PWM_PIN);     // enable main channel
    #ifdef RAMP_CH2
    DDRB |= (1 << ALT_PWM_PIN); // enable second channel
    #endif
    #ifdef RAMP_CH3
    // enable second PWM counter (OC1B) and third channel (FET, PB4)
    DDRB |= (1 << FET_PWM_PIN); // enable third channel (DDB4)
    #endif

    // Set timer to do PWM for correct output pin and set prescaler timing
    //TCCR0A = 0x23; // phase corrected PWM is 0x21 for PB1, fast-PWM is 0x23
    //TCCR0B = 0x01; // pre-scaler for timer (1 => 1, 2 => 8, 3 => 64...)
    //TCCR0A = FAST;
    // Set timer to do PWM for correct output pin and set prescaler timing
    TCCR0B = 0x01; // pre-scaler for timer (1 => 1, 2 => 8, 3 => 64...)

    #ifdef RAMP_CH3
    // Second PWM counter is ... weird
    TCCR1 = _BV (CS10);
    GTCCR = _BV (COM1B1) | _BV (PWM1B);
    OCR1C = 255;  // Set ceiling value to maximum
    #endif
}

int main(void)
{
    hw_setup();

    #ifdef CONFIG_MODE
    uint8_t mode_override = 0;
    // Read config values and saved state
    restore_state();
    #endif

    // check button press time, unless the mode is overridden
    if (! long_press) {
        // Indicates they did a short press, go to the next mode
        // We don't care what the fast_presses value is as long as it's over 15
        fast_presses = (fast_presses+1) & 0x1f;
        next_mode(); // Will handle wrap arounds
    } else {
        // Long press, use memorized level
        // ... or reset to the first mode
        fast_presses = 0;
        ramp_level = 1;
        ramp_dir = 1;
        next_mode_num = 255;
        mode_idx = 0;
        #ifdef MEMORY
        #ifdef MEMTOGGLE
        if (memory) { mode_override = MEMORY; }
        #else
        mode_override = MEMORY;
        #endif  // ifdef MEMTOGGLE
        #endif  // ifdef MEMORY
    }
    long_press = 0;
    #ifdef MEMORY
    save_mode();
    #endif

    // Turn features on or off as needed
    #ifdef VOLTAGE_MON
    #ifndef THERMAL_REGULATION
    ADC_on();
    #endif
    #else
    ADC_off();
    #endif

    uint8_t mode;
    #ifdef VOLTAGE_MON
    uint8_t lowbatt_cnt = 0;
    #endif
    #ifdef THERMAL_REGULATION
    uint8_t temperatures[THERM_HISTORY_SIZE];
    uint8_t overheat_count = 0;
    uint8_t underheat_count = 0;
    uint8_t first_temp_reading = 1;
    #endif
    uint8_t first_loop = 1;
    uint8_t loop_count = 0;
    while(1) {
        if (mode_idx < sizeof(modes)) mode = modes[mode_idx];
        else mode = mode_idx;

        #if defined(VOLTAGE_MON) && defined(THERMAL_REGULATION)
        // make sure a voltage reading has started, for LVP purposes
        ADC_on();
        #endif


        if (0) {  // This can't happen
        }

        #ifdef CONFIG_MODE
        else if (fast_presses > 15) {
            config_mode(&mode_override);
            // if config mode ends with no changes,
            // pretend this is the first loop
            continue;
        }
        #endif  // ifdef CONFIG_MODE

        #ifdef MEMORY
        // memorized level
        else if (mode_override == MEMORY) {
            // only do this once
            mode_override = 0;

            // moon mode for half a second
            set_mode(1);
            // if the user taps quickly, go to the real moon mode
            next_mode_num = 1;

            _delay_500ms();

            // if they didn't tap quickly, go to the memorized mode/level
            mode_idx = saved_mode_idx;
            ramp_level = saved_ramp_level;
            // remember for next time
            save_mode();
            // restart as if this were the first loop
            continue;
        }
        #endif

        // smooth ramp mode, lets user select any output level
        if (mode == RAMP) {
            ramp_mode();
        }

        else if (mode == STEADY) {
            // normal flashlight mode
            if (first_loop) {
                set_mode(ramp_level);
                target_level = ramp_level;
            }
            // User has 0.5s to tap again to advance to the next mode
            //next_mode_num = 255;
            _delay_500ms();
            // After a delay, assume user wants to adjust ramp
            // instead of going to next mode (unless they're
            // tapping rapidly, in which case we should advance to turbo)
            next_mode_num = 0;
        }

        else if (mode == TURBO) {
            // turbo is special because it's easier to handle that way
            if (first_loop) {
                set_mode(RAMP_SIZE);
                target_level = RAMP_SIZE;
            }
            //next_mode_num = 255;
            _delay_500ms();
            // go back to the previously-memorized level
            // if the user taps after a delay,
            // instead of advancing to blinkies
            // (allows something similar to "momentary" turbo)
            next_mode_num = 1;
        }

        #ifdef STROBE
        else if (mode == STROBE) {
            // 10Hz tactical strobe
            strobe(33/4,67/4);
        }
        #endif // ifdef STROBE

        #ifdef POLICE_STROBE
        else if (mode == POLICE_STROBE) {
            // police-like strobe
            strobe(20/4,40/4);
            strobe(40/4,80/4);
        }
        #endif // ifdef POLICE_STROBE

        #ifdef RANDOM_STROBE
        else if (mode == RANDOM_STROBE) {
            // pseudo-random strobe
            uint8_t ms = (34 + (pgm_rand() & 0x3f))>>2;
            //strobe(ms, ms);
            set_level(RAMP_SIZE);
            _delay_4ms(ms);
            set_level(0);
            _delay_4ms(ms);
            //strobe(ms, ms);
        }
        #endif // ifdef RANDOM_STROBE

        #ifdef BIKING_MODE
        else if (mode == BIKING_MODE) {
            // 2-level stutter beacon for biking and such
            biking_mode(RAMP_SIZE/2, RAMP_SIZE);
        }
        #endif  // ifdef BIKING_MODE

        #ifdef BIKING_MODE2
        else if (mode == BIKING_MODE2) {
            // 2-level stutter beacon for biking and such
            biking_mode(RAMP_SIZE/4, RAMP_SIZE/2);
        }
        #endif  // ifdef BIKING_MODE

        #ifdef SOS
        else if (mode == SOS) { SOS_mode(); }
        #endif // ifdef SOS

        #ifdef HEART_BEACON
        else if (mode == HEART_BEACON) {
            set_mode(RAMP_SIZE);
            _delay_4ms(1);
            set_mode(0);
            _delay_4ms(250/4);
            set_mode(RAMP_SIZE);
            _delay_4ms(1);
            set_mode(0);
            _delay_4ms(750/4);
        }
        #endif

        #ifdef PARTY_STROBE12
        else if (mode == PARTY_STROBE12) {
            party_strobe_loop(1,79);
        }
        #endif

        #ifdef PARTY_STROBE24
        else if (mode == PARTY_STROBE24) {
            party_strobe_loop(0,41);
        }
        #endif

        #ifdef PARTY_STROBE60
        else if (mode == PARTY_STROBE60) {
            party_strobe_loop(0,15);
        }
        #endif

        #ifdef PARTY_VARSTROBE1
        else if (mode == PARTY_VARSTROBE1) {
            uint8_t j, speed;
            for(j=0; j<66; j++) {
                if (j<33) { speed = j; }
                else { speed = 66-j; }
                party_strobe(1,(speed+33-6)<<1);
            }
        }
        #endif

        #ifdef PARTY_VARSTROBE2
        else if (mode == PARTY_VARSTROBE2) {
            uint8_t j, speed;
            for(j=0; j<100; j++) {
                if (j<50) { speed = j; }
                else { speed = 100-j; }
                party_strobe(0, speed+9);
            }
        }
        #endif

        #ifdef BATTCHECK
        // battery check mode, show how much power is left
        else if (mode == BATTCHECK) {
            battcheck_mode();
        }
        #endif // ifdef BATTCHECK

        #ifdef GOODNIGHT
        // "good night" mode, slowly ramps down and shuts off
        else if (mode == GOODNIGHT) {
            goodnight_mode();
        }
        #endif // ifdef GOODNIGHT

        else {  // shouldn't happen  (compiler omits this entire clause)
        }
        fast_presses = 0;

        #ifdef VOLTAGE_MON
        monitor_voltage(mode, &lowbatt_cnt);
        #endif  // ifdef VOLTAGE_MON

        #ifdef THERMAL_REGULATION
        if ((mode == STEADY) || (mode == TURBO) || (mode == THERM_CALIBRATION_MODE)) {
            monitor_temperature(mode, temperatures,
                                &overheat_count, &underheat_count,
                                first_temp_reading, first_loop, &loop_count);
            first_temp_reading = 0;
        }
        #endif  // ifdef THERMAL_REGULATION

        first_loop = 0;
        loop_count ++;
    }
}
