#ifndef TK_ATTINY_H
#define TK_ATTINY_H
/*
 * Attiny portability header.
 * This helps abstract away the differences between various attiny MCUs.
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
 */

// Choose your MCU here, or in the main .c file, or in the build script
//#define ATTINY 13
//#define ATTINY 25

/******************** hardware-specific values **************************/
#if (ATTINY == 13)
    #define F_CPU 4800000UL
    //#define EEPSIZE 64
    #define V_REF REFS0
    #define BOGOMIPS 950
    #define DELAY_ZERO_TIME 252
    #define SWITCH_PORT  PINB  // PINA or PINB or PINC
    #define VOLTAGE_ADC_DIDR DIDR0  // this MCU only has one DIDR
#elif (ATTINY == 25)
    // TODO: Use 6.4 MHz instead of 8 MHz?
    #define F_CPU 8000000UL
    //#define EEPSIZE 128
    #define V_REF REFS1
    #define BOGOMIPS (F_CPU/4000)
    #define ADMUX_THERM 0b10001111
    #define DELAY_ZERO_TIME 1020
    #define SWITCH_PORT  PINB  // PINA or PINB or PINC
    #define VOLTAGE_ADC_DIDR DIDR0  // this MCU only has one DIDR
#elif (ATTINY == 85)
    // TODO: Use 6.4 MHz instead of 8 MHz?
    #define F_CPU 8000000UL
    //#define EEPSIZE 512
    #define V_REF REFS1
    #define BOGOMIPS (F_CPU/4000)
    // (1 << V_REF) | (0 << ADLAR) | (VCC_CHANNEL)
    // (1 << V_REF) | (0 << ADLAR) | (THERM_CHANNEL)
    #define ADMUX_THERM 0b10001111
    #define DELAY_ZERO_TIME 1020
    #define SWITCH_PORT  PINB  // PINA or PINB or PINC
    #define VOLTAGE_ADC_DIDR DIDR0  // this MCU only has one DIDR
#elif (ATTINY == 1634)
    #define F_CPU 8000000UL
    #define V_REF REFS1
    #define BOGOMIPS (F_CPU/4000)
    // (1 << V_REF) | (THERM_CHANNEL)
    // DS table 19-3, 19-4, internal sensor / 1.1V ref
    #define ADMUX_THERM 0b10001110
    #define DELAY_ZERO_TIME 1020
    //#define SWITCH_PORT  PINA  // set this in hwdef
    //#define VOLTAGE_ADC_DIDR DIDR0  // set this in hwdef
#else
    #error Hey, you need to define ATTINY.
#endif

// auto-detect eeprom size from avr-libc headers
#ifndef EEPSIZE
#ifdef E2SIZE
#define EEPSIZE E2SIZE
#else
#define EEPSIZE (E2END+1)
#endif
#endif


#include <avr/interrupt.h>

/******************** I/O pin and register layout ************************/
#ifdef HWDEFFILE
#include "tk.h"
#include incfile(HWDEFFILE)
#endif

#if 0  // placeholder

#elif defined(NANJG_LAYOUT)
#include "hwdef-nanjg.h"

#elif defined(FET_7135_LAYOUT)
#include "hwdef-FET_7135.h"

#elif defined(TRIPLEDOWN_LAYOUT)
#include "hwdef-Tripledown.h"

#elif defined(H17FX_LAYOUT)
#include "hwdef-H17Fx.h"

#elif defined(FERRERO_ROCHER_LAYOUT)
#include "hwdef-Ferrero_Rocher.h"

#endif  // no more recognized driver types

#ifndef LAYOUT_DEFINED
#error Hey, you need to define an I/O pin layout.
#endif

#if (ATTINY==13)
  // no changes needed
#elif (ATTINY==25) || (ATTINY==45) || (ATTINY==85)
  // use clock_prescale_set(n) instead; it's safer
  //#define CLOCK_DIVIDER_SET(n) {CLKPR = 1<<CLKPCE; CLKPR = n;}
#elif (ATTINY==1634)
  // make it a NOP for now
  // FIXME
  //#define clock_prescale_set(x) ((void)0)
  //#define clock_prescale_set(n) {cli(); CCP = 0xD8; CLKPR = n; sei();}
  //#define clock_prescale_set(n) {cli(); CCP = 0xD8; CLKPR = n; sei();}
  inline void clock_prescale_set(uint8_t n) {cli(); CCP = 0xD8; CLKPR = n; sei();}
  typedef enum
  {
      clock_div_1 = 0,
      clock_div_2 = 1,
      clock_div_4 = 2,
      clock_div_8 = 3,
      clock_div_16 = 4,
      clock_div_32 = 5,
      clock_div_64 = 6,
      clock_div_128 = 7,
      clock_div_256 = 8
  } clock_div_t;

#else
#error Unable to define MCU macros.
#endif

#endif  // TK_ATTINY_H
