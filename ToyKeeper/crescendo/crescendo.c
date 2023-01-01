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
 * ATtiny13A/25/45/85 DIAGRAM
 * --------------------------
 *   NOTE: The ATtiny13A does not support PWM on pin 3.
 *
 *               ----
 *       Reset -|1  8|- VCC
 *         OTC -|2  7|- Voltage ADC
 *   PWM (PB4) -|3  6|- PWM (PB1)
 *         GND -|4  5|- PWM (PB0)
 *               ----
 *
 * FUSES
 * -----
 *   (check bin/flash*.sh for recommended values)
 *
 * CALIBRATION
 * -----------
 *   To find out what values to use, flash the driver with battcheck.hex
 *   and hook the light up to each voltage you need a value for.  This is
 *   much more reliable than attempting to calculate the values from a
 *   theoretical formula.
 *
 *   Same for off-time capacitor values.  Measure, don't guess.
 */

// User-configurable options
#include "config-default.h"

// Specific settings/overrides for known driver types
// config file name (set it here or define it at the gcc command line)
//#define CONFIGFILE cfg-blf-q8.h

#include "tk.h"
#include incfile(CONFIGFILE)

// Helper macros related to config
#include "helper-macros.h"

// Ramp config
#include "ramp-config.h"

// Calibrate voltage and OTC in this file:
#include "tk-calibration.h"

// Also, assign I/O pins in this file:
#include "tk-attiny.h"

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
#include "ramping.h"

#ifdef OFFTIME
#include "offtime.h"
#endif

#ifdef VOLTAGE_MON
#include "voltage-monitor.h"
#endif

#ifdef THERMAL_REGULATION
#include "thermal-regulation.h"
#endif

#ifdef MEMORY
#include "memory.h"
#endif

#ifdef CONFIG_MODE
#include "config-mode.h"
#endif

#ifdef BIKING_MODE
#include "biking-mode.h"
#endif

#ifdef GOODNIGHT
#include "goodnight-mode.h"
#endif

#ifdef SOS
#include "sos-mode.h"
#endif

#ifdef HEART_BEACON
#include "heart-beacon-mode.h"
#endif

#if defined(TACTICAL_STROBES) || defined(PARTY_STROBES)
#include "strobes.h"
#endif

/********* Include all the app logic source files *********/
// (is a bit weird to do things this way,
//  but it saves a lot of space by letting us use the -fwhole-program flag)

#include "misc.c"
#include "load-save.c"
#include "ramping.c"

#ifdef OFFTIME
#include "offtime.c"
#endif

#ifdef VOLTAGE_MON
#include "voltage-monitor.c"
#endif

#ifdef THERMAL_REGULATION
#include "thermal-regulation.c"
#endif

#ifdef MEMORY
#include "memory.c"
#endif

#ifdef CONFIG_MODE
#include "config-mode.c"
#endif

#ifdef BIKING_MODE
#include "biking-mode.c"
#endif

#ifdef GOODNIGHT
#include "goodnight-mode.c"
#endif

#ifdef SOS
#include "sos-mode.c"
#endif

#ifdef HEART_BEACON
#include "heart-beacon-mode.c"
#endif

#if defined(TACTICAL_STROBES) || defined(PARTY_STROBES)
#include "strobes.c"
#endif


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

    #ifdef ENABLE_PWM1B
    // Second PWM counter (PB4/OCR1B) is ... weird
    TCCR1 = _BV (CS10);
    GTCCR = _BV (COM1B1) | _BV (PWM1B);
    OCR1C = 255;  // Set ceiling value to maximum
    #endif
}

// Just in case (SRAM could have partially decayed).
// We can't really do anything about:
//  * `long_press`
//  * `fast_presses`
inline void validate_noinits() {
    // `mode_id`, `next_mode_id`
    // We should reset these together because corruption in either one would
    // invalidate normal operation w.r.t. the other.
    if (
        !(
            (mode_id < mode_cycle_length)
            // If `mode_id` has been overridden to a `mode_num_e`, then the
            // next mode would be `STEADY_E` anyway as per `next_mode()`
            //|| (RAMP_E <= mode_id && mode_id < INVALID_MODE_MIN_E)
        )
        || !(
            (next_mode_id < mode_cycle_length)
            || (RAMP_E <= next_mode_id && next_mode_id < INVALID_MODE_MIN_E)
            || (next_mode_id == NEXT_MODE_IN_CYCLE)
        )
    ) {
        // We don't need to bother setting `mode_id` because it will be set to
        // steady immediately afterward in `next_mode()` if we override via
        // `next_mode_id`
        next_mode_id = STEADY_IDX;
    }

    // `ramp_level`:
    ramp_level = (0 < ramp_level && ramp_level <= MAX_LEVEL) ? ramp_level : 1;

    // `ramp_dir`:
    ramp_dir = (ramp_dir == 1) ? 1 : -1;
}

int main(void)
{
    #ifdef OFFTIME
    // check the OTC immediately before it has a chance to charge or discharge
    uint8_t cap_val = read_otc();  // save it for later
    #endif

    hw_setup();

    #ifdef MEMORY
    volatile uint8_t mode_override = 0;
    #endif

    #if defined(MEMORY_WL) || defined(CONFIG_MODE)
    // Read config values and saved state
    restore_state();
    #endif

    // check button press time
    #ifdef OFFTIME
    if (cap_val < CAP_SHORT) {
    #else
    if (long_press) {
    #endif
        // Long press, use memorized level
        // ... or reset to the first mode
        fast_presses = 0;
        mode_id = RAMP_IDX;
        next_mode_id = NEXT_MODE_IN_CYCLE;
        ramp_level = 1;
        ramp_dir = 1;
        #ifdef MEMORY
        #ifdef MEMTOGGLE
        if (memory)
            mode_override = MEMORIZED_MODE_E;
        #else
        mode_override = MEMORIZED_MODE_E;
        #endif
        #endif
    } else {
        // Fast press, go to the next mode
        validate_noinits();
        next_mode();  // Will handle wrap arounds

        // For config mode, we only care whether the `fast_presses` value is
        // over 15
        //
        // (This will wrap after pressing the button 255 times, but I think
        // it's probably fine to not handle this as there's no real reason to
        // "accidentally" press the button that many times...)
        fast_presses++;

        #ifdef MEMORY_WL
        save_state_wl();
        #endif
    }

    // reset offtime indicator
    #ifdef OFFTIME
    charge_otc();
    #else
    long_press = 0;
    #endif

    // Turn features on or off as needed
    #ifdef VOLTAGE_MON
    #ifndef THERMAL_REGULATION
    ADC_on();
    #endif
    #else
    ADC_off();
    #endif

    #ifdef VOLTAGE_PROTECTION
    uint8_t lowbatt_cnt = 0;
    #endif
    #ifdef THERMAL_REGULATION
    uint8_t temperatures[THERM_HISTORY_SIZE];
    uint8_t overheat_count = 0;
    uint8_t underheat_count = 0;
    uint8_t first_temp_reading = 1;
    uint8_t loop_count = 0;
    #endif
    volatile uint8_t first_loop = 1;
    volatile mode_num_e mode;
    while (1) {
        if (mode_id < mode_cycle_length)
            mode = mode_cycle[mode_id];
        else
            mode = mode_id;

        #if defined(VOLTAGE_MON) && defined(THERMAL_REGULATION)
        // make sure a voltage reading has started, for LVP purposes
        ADC_on();
        #endif

        /* ==== Special Modes ============================================== */
        if (0) {}

        #ifdef CONFIG_MODE
        else if (fast_presses > 15) {
            _delay_s();        // wait for user to stop fast-pressing button
            fast_presses = 0;  // exit this mode after one use
            config_mode();
            // if config mode ends with no changes,
            // pretend this is the first loop
            continue;
        }
        #endif

        #ifdef MEMORY
        // memorized level
        else if (mode_override == MEMORIZED_MODE_E) {
            // only do this once
            mode_override = 0;
            memory_override();
            // restart as if this were the first loop
            continue;
        }
        #endif

        /* ==== Normal Modes =============================================== */
        // smooth ramp mode, lets user select any output level
        if (0) {}

        else if (mode == RAMP_E) {
            ramp_mode();
        }

        else if (mode == STEADY_E) {
            if (first_loop) {
                steady_mode();
                delay_set_next_mode(RAMP_IDX);
            } else {
                _delay_s();  // wait between voltage/temperature checks
            }
        }

        else if (mode == TURBO_E) {
            if (first_loop) {
                turbo_mode();
                delay_set_next_mode(STEADY_IDX);
            } else {
                _delay_s();  // wait between voltage/temperature checks
            }
        }

        #ifdef BATTCHECK
        else if (mode == BATTCHECK_E) {
            if (first_loop)
                delay_set_next_mode(STEADY_IDX);
            battcheck_mode();
        }
        #endif

        #ifdef GOODNIGHT
        else if (mode == GOODNIGHT_E) {
            if (first_loop)
                delay_set_next_mode(STEADY_IDX);
            goodnight_mode();
        }
        #endif

        #ifdef BIKING_MODE
        else if (mode == BIKING_MODE_E) {
            if (first_loop)
                delay_set_next_mode(STEADY_IDX);
            biking_mode(ramp_level);
        }
        #endif

        #ifdef SOS
        else if (mode == SOS_E) {
            if (first_loop)
                delay_set_next_mode(STEADY_IDX);
            SOS_mode(ramp_level);
        }
        #endif

        #ifdef RANDOM_STROBE
        else if (mode == RANDOM_STROBE_E) {
            if (first_loop)
                delay_set_next_mode(STEADY_IDX);
            random_strobe();
        }
        #endif

        #ifdef POLICE_STROBE
        else if (mode == POLICE_STROBE_E) {
            if (first_loop)
                delay_set_next_mode(STEADY_IDX);
            police_strobe();
        }
        #endif

        #ifdef STROBE
        else if (mode == STROBE_E) {
            if (first_loop)
                delay_set_next_mode(STEADY_IDX);
            tactical_strobe();
        }
        #endif

        #ifdef HEART_BEACON
        else if (mode == HEART_BEACON_E) {
            if (first_loop)
                delay_set_next_mode(STEADY_IDX);
            heart_beacon_mode();
        }
        #endif

        #ifdef PARTY_STROBE12
        else if (mode == PARTY_STROBE12_E) {
            if (first_loop)
                delay_set_next_mode(STEADY_IDX);
            party_strobe_12();
        }
        #endif

        #ifdef PARTY_STROBE24
        else if (mode == PARTY_STROBE24_E) {
            if (first_loop)
                delay_set_next_mode(STEADY_IDX);
            party_strobe_24();
        }
        #endif

        #ifdef PARTY_STROBE60
        else if (mode == PARTY_STROBE60_E) {
            if (first_loop)
                delay_set_next_mode(STEADY_IDX);
            party_strobe_60();
        }
        #endif

        #ifdef PARTY_VARSTROBE1
        else if (mode == PARTY_VARSTROBE1_E) {
            if (first_loop)
                delay_set_next_mode(STEADY_IDX);
            party_varstrobe_1();
        }
        #endif

        #ifdef PARTY_VARSTROBE2
        else if (mode == PARTY_VARSTROBE2_E) {
            if (first_loop)
                delay_set_next_mode(STEADY_IDX);
            party_varstrobe_2();
        }
        #endif

        /* ==== Protection/Cleanup ========================================= */
        #ifdef VOLTAGE_PROTECTION
        monitor_voltage(mode, &lowbatt_cnt);
        #endif

        #ifdef THERMAL_REGULATION
        if ((mode == STEADY_E) || (mode == TURBO_E)
            #ifdef THERM_CALIBRATION_MODE
            || (mode == THERM_CALIBRATION_MODE_E)
            #endif
        ) {
            monitor_temperature(
                mode,
                temperatures,
                &overheat_count,
                &underheat_count,
                first_temp_reading,
                first_loop,
                &loop_count
            );
            first_temp_reading = 0;
        }
        loop_count++;
        #endif

        fast_presses = 0;
        first_loop = 0;
    }
}
