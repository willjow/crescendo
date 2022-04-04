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

// Config option variables
#ifdef MEMTOGGLE
uint8_t memory;
#endif
#ifdef THERMAL_REGULATION
uint8_t therm_ceil = DEFAULT_THERM_CEIL;
#endif
// Other state variables
uint8_t eepos;
uint8_t saved_mode_idx = 0;
uint8_t saved_ramp_level = 1;
// counter for entering config mode
// (needs to be remembered while off, but only for up to half a second)
uint8_t fast_presses __attribute__ ((section (".noinit")));
uint32_t long_press __attribute__ ((section (".noinit")));
// current or last-used mode number
uint8_t mode_idx __attribute__ ((section (".noinit")));
uint8_t ramp_level __attribute__ ((section (".noinit")));
int8_t ramp_dir __attribute__ ((section (".noinit")));
uint8_t next_mode_num __attribute__ ((section (".noinit")));
uint8_t target_level;  // ramp level before thermal stepdown
uint8_t actual_level;  // last ramp level activated

enum mode_nums {
    RAMP_E = MAX_MODES,
    STEADY_E,
    TURBO_E,
#ifdef USE_BATTCHECK
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
    MEMORY_E,
#endif
#ifdef THERM_CALIBRATION_MODE
    THERM_CALIBRATION_MODE_E,
#endif
};

#define RAMP_IDX 0
#define STEADY_IDX 1
#define TURBO_IDX 2
#define DISABLE_MODE_OVERRIDE 255
uint8_t mode_cycle[] = {
    RAMP_E,
    STEADY_E,
    TURBO_E,
#ifdef USE_BATTCHECK
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
