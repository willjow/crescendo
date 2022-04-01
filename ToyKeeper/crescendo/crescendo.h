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
uint16_t long_press __attribute__ ((section (".noinit")));
// current or last-used mode number
uint8_t mode_idx __attribute__ ((section (".noinit")));
uint8_t ramp_level __attribute__ ((section (".noinit")));
int8_t ramp_dir __attribute__ ((section (".noinit")));
uint8_t next_mode_num __attribute__ ((section (".noinit")));
uint8_t target_level;  // ramp level before thermal stepdown
uint8_t actual_level;  // last ramp level activated

uint8_t modes[] = {
    RAMP, STEADY, TURBO,
#ifdef USE_BATTCHECK
    BATTCHECK,
#endif
#ifdef GOODNIGHT
    GOODNIGHT,
#endif
#ifdef BIKING_MODE2
    BIKING_MODE2,
#endif
#ifdef BIKING_MODE
    BIKING_MODE,
#endif
#ifdef RANDOM_STROBE
    RANDOM_STROBE,
#endif
#ifdef POLICE_STROBE
    POLICE_STROBE,
#endif
#ifdef STROBE
    STROBE,
#endif
#ifdef HEART_BEACON
    HEART_BEACON,
#endif
#ifdef PARTY_STROBE12
    PARTY_STROBE12,
#endif
#ifdef PARTY_STROBE24
    PARTY_STROBE24,
#endif
#ifdef PARTY_STROBE60
    PARTY_STROBE60,
#endif
#ifdef PARTY_VARSTROBE1
    PARTY_VARSTROBE1,
#endif
#ifdef PARTY_VARSTROBE2
    PARTY_VARSTROBE2,
#endif
#ifdef SOS
    SOS,
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

#endif
