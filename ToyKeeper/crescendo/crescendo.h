/*
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
 */

#ifndef CRESCENDO_H
#define CRESCENDO_H

#include <avr/pgmspace.h>

/*
 * global variables
 */

#ifndef OFFTIME
// Bytes to use for checking whether the SRAM has retained its data.
// We basically set these all to 0, then check whether any bits have inverted
// to 1 upon startup. We declare a long press if there are any inversions and
// declare a short press otherwise.
//
// More bytes should theoretically decrease the probability of falsely
// declaring a short press as there are more chances for noise to cause
// inversions after the voltage decreases below the data-retention threshold.
//
// NOTE: ToyKeeper empirically determined that there's no advantage of using 32
// bits here over 8: https://budgetlightforum.com/comment/1101037#comment-1101037
//
// Regardless, I'll leave this as 32 bits as long as it fits because the theory
// remains the same.
volatile uint32_t long_press __attribute__ ((section (".noinit"))) __attribute__ ((used));
#endif
volatile uint8_t fast_presses __attribute__ ((section (".noinit"))) __attribute__ ((used));

volatile uint8_t mode_id __attribute__ ((section (".noinit"))) __attribute__ ((used));
volatile uint8_t next_mode_id __attribute__ ((section (".noinit"))) __attribute__ ((used));

volatile int8_t ramp_dir __attribute__ ((section (".noinit"))) __attribute__ ((used));
volatile uint8_t ramp_level __attribute__ ((section (".noinit"))) __attribute__ ((used));

uint8_t actual_level;  // last ramp level activated

// Maximum number of modes; modes will mapped to ints via enum starting
// at MAX_MODES. An equally large range should be reserved for indexes into the
// mode_cycle array (starting at 0 of course). We also need to reserve one
// value (currently 255) to serve as the indicator that mode overrides should
// be ignored and we should instead just advance to the next mode in the cycle.
#define MAX_MODES 127
#define NEXT_MODE_IN_CYCLE 255

typedef enum mode_num {
    RAMP_E = MAX_MODES,
    STEADY_E,
    TURBO_E,
#ifdef BATTCHECK
    BATTCHECK_E,
#endif
#ifdef GOODNIGHT
    GOODNIGHT_E
#endif
#ifdef BIKING_MODE
    BIKING_MODE_E,
#endif
#ifdef SOS
    SOS_E,
#endif
#ifdef RANDOM_STROBE
    RANDOM_STROBE_E,
#endif
#ifdef POLICE_STROBE
    POLICE_STROBE_E,
#endif
#ifdef STROBE
    STROBE_E,
#endif
#ifdef HEART_BEACON
    HEART_BEACON_E,
#endif
#ifdef PARTY_STROBE12
    PARTY_STROBE12_E,
#endif
#ifdef PARTY_STROBE24
    PARTY_STROBE24_E,
#endif
#ifdef PARTY_STROBE60
    PARTY_STROBE60_E,
#endif
#ifdef PARTY_VARSTROBE1
    PARTY_VARSTROBE1_E,
#endif
#ifdef PARTY_VARSTROBE2
    PARTY_VARSTROBE2_E,
#endif
#ifdef MEMORY
    MEMORIZED_MODE_E,
#endif
#ifdef THERM_CALIBRATION_MODE
    THERM_CALIBRATION_MODE_E,
#endif
    INVALID_MODE_MIN_E
} mode_num_e;

const uint8_t num_modes = INVALID_MODE_MIN_E - MAX_MODES;

#define RAMP_IDX 0
#define STEADY_IDX 1
#define TURBO_IDX 2
const mode_num_e mode_cycle[] = {
    RAMP_E,
    STEADY_E,
    TURBO_E,
#ifdef BATTCHECK
    BATTCHECK_E,
#endif
#ifdef GOODNIGHT
    GOODNIGHT_E,
#endif
#ifdef BIKING_MODE
    BIKING_MODE_E,
#endif
#ifdef SOS
    SOS_E,
#endif
#ifdef RANDOM_STROBE
    RANDOM_STROBE_E,
#endif
#ifdef POLICE_STROBE
    POLICE_STROBE_E,
#endif
#ifdef STROBE
    STROBE_E,
#endif
#ifdef HEART_BEACON
    HEART_BEACON_E,
#endif
#ifdef PARTY_STROBE12
    PARTY_STROBE12_E,
#endif
#ifdef PARTY_STROBE24
    PARTY_STROBE24_E,
#endif
#ifdef PARTY_STROBE60
    PARTY_STROBE60_E,
#endif
#ifdef PARTY_VARSTROBE1
    PARTY_VARSTROBE1_E,
#endif
#ifdef PARTY_VARSTROBE2
    PARTY_VARSTROBE2_E,
#endif
};

const uint8_t mode_cycle_length = sizeof(mode_cycle) / sizeof(mode_cycle[0]);

// Modes (gets set when the light starts up based on saved config values)
PROGMEM const uint8_t ramp_ch1[]  = { RAMP_CH1 };
#ifdef RAMP_CH2
PROGMEM const uint8_t ramp_ch2[] = { RAMP_CH2 };
#endif
#ifdef RAMP_CH3
PROGMEM const uint8_t ramp_ch3[] = { RAMP_CH3 };
#endif
#define RAMP_SIZE  sizeof(ramp_ch1)
#define MAX_LEVEL RAMP_SIZE

#endif
