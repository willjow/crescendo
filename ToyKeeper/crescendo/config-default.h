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

#ifndef CONFIG_DEFAULT_H
#define CONFIG_DEFAULT_H

// Choose your MCU here, or in the build script
//#define ATTINY 13
//#define ATTINY 25

// Pick your driver type:
#define FET_7135_LAYOUT
//#define NANJG_LAYOUT
//#define TRIPLEDOWN_LAYOUT
//
/*
 * =========================================================================
 * Settings to modify per driver
 */

#define VOLTAGE_MON         // Comment out to disable LVP and battcheck
#define THERMAL_REGULATION  // Comment out to disable thermal regulation
#define MAX_THERM_CEIL 70   // Highest allowed temperature ceiling
#define DEFAULT_THERM_CEIL 50  // Temperature limit when unconfigured

// How many ms should it take to ramp all the way up?
// (recommended values 2000 to 5000 depending on personal preference)
#define RAMP_TIME  5000

// How long to wait at ramp ends, and
// how long the user has to continue multi-taps after the light comes on
// (higher makes it slower and easier to do double-taps / triple-taps,
//  lower makes the UI faster)
// (recommended values 250 to 750)
#define INPUT_WAIT_TIME 333

// Enable battery indicator mode?
#ifdef VOLTAGE_MON
#define USE_BATTCHECK
#endif
// Choose a battery indicator style
//#define BATTCHECK_4bars  // up to 4 blinks
//#define BATTCHECK_8bars  // up to 8 blinks
#define BATTCHECK_VpT      // Volts + tenths

// output to use for blinks on battery check (and other modes)
#define BLINK_BRIGHTNESS    (MAX_LEVEL/4)
// 4ms units per normal-speed blink
#define BLINK_SPEED         (1000/4)
#define BLINK_ONTIME        (BLINK_SPEED * 2 / 12)
#define BLINK_OFFTIME       (BLINK_SPEED * 3 / 12)
#define BLINK_SPACE         (BLINK_SPEED * 8 / 12)

// Uncomment this if you want the ramp to stop when it reaches maximum
//#define STOP_AT_TOP     HOP_ON_POP
// Uncomment this if you want it to blink when it reaches maximum
#define BLINK_AT_TOP

// Enabled Modes
#ifdef VOLTAGE_MON
#define BATTCHECK
#endif

//#define MEMORY
#ifdef MEMORY
//#define MEMTOGGLE             // runtime config for memory (requires MEMORY)
#endif

#ifdef THERMAL_REGULATION
#define THERM_CALIBRATION_MODE  // let user configure temperature limit
#endif

#define BIKING_MODE             // steady on with pulses at 1Hz
#define FULL_BIKING_MODE        // comment out to use minimal version
                                // instead (decreases program size)

//#define TACTICAL_STROBES      // Required for any of the tactical strobes
//#define STROBE                // Simple tactical strobe
//#define POLICE_STROBE         // 2-speed tactical strobe
//#define RANDOM_STROBE         // variable-speed tactical strobe

//#define PARTY_STROBES         // Required for any of the party strobes
//#define PARTY_STROBE12        // 12Hz party strobe
//#define PARTY_STROBE24        // 24Hz party strobe
//#define PARTY_STROBE60        // 60Hz party strobe
//#define PARTY_VARSTROBE1      // variable-speed party strobe (slow)
//#define PARTY_VARSTROBE2      // variable-speed party strobe (fast)

//#define SOS                   // distress signal
//#define HEART_BEACON          // 1Hz heartbeat-pattern beacon
//#define GOODNIGHT             // hour-long ramp down then poweroff


#if defined(MEMTOGGLE) || defined(THERM_CALIBRATION_MODE)
#define CONFIG_MODE
#endif

#endif
