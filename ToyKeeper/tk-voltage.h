#ifndef TK_VOLTAGE_H
#define TK_VOLTAGE_H
/*
 * Voltage / battcheck functions.
 *
 * Copyright (C) 2015 Selene Scriven
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

#include "tk-attiny.h"
#include "tk-calibration.h"

#if defined(TEMPERATURE_MON) || defined(THERMAL_REGULATION)
#ifdef TEMP_10bit
#define NEED_ADC_10bit
#define get_temperature read_adc_10bit
#else
#define NEED_ADC_8bit
#define get_temperature read_adc_8bit
#endif

inline void ADC_on_temperature() {
    // TODO: (?) enable ADC Noise Reduction Mode, Section 17.7 on page 128
    //       (apparently can only read while the CPU is in idle mode though)
    // select ADC4 by writing 0b00001111 to ADMUX
    // 1.1v reference, left-adjust, ADC4
    #ifdef TEMP_10bit
    ADMUX  = (1 << V_REF) | (0 << ADLAR) | TEMP_CHANNEL;
    #else
    ADMUX  = (1 << V_REF) | (1 << ADLAR) | TEMP_CHANNEL;
    #endif
    // disable digital input on ADC pin to reduce power consumption
    //DIDR0 |= (1 << TEMP_DIDR);
    // enable, start, prescale
    ADCSRA = (1 << ADEN ) | (1 << ADSC ) | ADC_PRSCL;
}
#endif  // TEMPERATURE_MON


#ifdef VOLTAGE_MON

#ifdef VCC_REF
#define NEED_ADC_10bit
#define get_voltage read_adc_10bit
#define ADC_MAX 0x0000  // ADC values are decreasing so "max" is actually min
#define VOLTAGE_TYPE uint16_t
#else  // #ifdef VCC_REF
#define NEED_ADC_8bit
#define get_voltage read_adc_8bit
#define ADC_MAX 0xFF
#define VOLTAGE_TYPE uint8_t
#endif  // #ifdef VCC_REF

inline void ADC_on() {
    #ifdef ADC_DIDR
    // disable digital input on ADC pin to reduce power consumption
    DIDR0 |= (1 << ADC_DIDR);
    #endif

    #ifdef VCC_REF
    // right-adjust, set ADC reference/input channel
    ADMUX  = (0 << ADLAR) | ADC_REF | ADC_CHANNEL;
    #else
    // left-adjust, set ADC reference/input channel
    ADMUX  = (1 << ADLAR) | ADC_REF | ADC_CHANNEL;
    #endif
    // enable, start, prescale
    ADCSRA = (1 << ADEN ) | (1 << ADSC ) | ADC_PRSCL;
}

#else  // #ifdef VOLTAGE_MON
inline void ADC_off() {
    ADCSRA &= ~(1<<7); //ADC off
}
#endif // #ifdef VOLTAGE_MON

#ifdef NEED_ADC_8bit
uint8_t read_adc_8bit() {
    // Start conversion
    ADCSRA |= (1 << ADSC);
    // Wait for completion
    while (ADCSRA & (1 << ADSC));
    // Send back the result
    return ADCH;
}
#endif

#ifdef NEED_ADC_10bit
uint16_t read_adc_10bit() {
    // Start conversion
    ADCSRA |= (1 << ADSC);
    // Wait for completion
    while (ADCSRA & (1 << ADSC));
    // Send back the result
    //return (ADCH<<8) | (ADCL);  // ADLAR=0
    //return (ADCH<<2) | (ADCL>>6);  // ADLAR=1
    ADCSRA |= 0x10;  // clear ADIF flag, else only first reading works
    return ADC;  // ADLAR=0
}
#endif

#ifdef BATTCHECK
// Store calibrated ADC reference values and number of blinks in the same array
PROGMEM const VOLTAGE_TYPE voltage_blinks[] = {
#if defined(BATTCHECK_4bars)
               // 0 blinks for less than 1%
    ADC_0p,    // 1 blink  for 1%-25%
    ADC_25p,   // 2 blinks for 25%-50%
    ADC_50p,   // 3 blinks for 50%-75%
    ADC_75p,   // 4 blinks for 75%-100%
    ADC_100p,  // 5 blinks for >100%
    ADC_MAX,   // Ceiling, don't remove  (6 blinks means "error")
#elif defined(BATTCHECK_8bars)
               // 0 blinks for less than 1%
    ADC_30,    // 1 blink  for 1%-12.5%
    ADC_33,    // 2 blinks for 12.5%-25%
    ADC_35,    // 3 blinks for 25%-37.5%
    ADC_37,    // 4 blinks for 37.5%-50%
    ADC_38,    // 5 blinks for 50%-62.5%
    ADC_39,    // 6 blinks for 62.5%-75%
    ADC_40,    // 7 blinks for 75%-87.5%
    ADC_41,    // 8 blinks for 87.5%-100%
    ADC_42,    // 9 blinks for >100%
    ADC_MAX,   // Ceiling, don't remove  (10 blinks means "error")
#elif defined(BATTCHECK_VpT)
    // 0 blinks for ... (shouldn't happen)
    ADC_25,  (2<<5)+5,
    ADC_26,  (2<<5)+6,
    ADC_27,  (2<<5)+7,
    ADC_28,  (2<<5)+8,
    ADC_29,  (2<<5)+9,
    ADC_30,  (3<<5)+0,
    ADC_31,  (3<<5)+1,
    ADC_32,  (3<<5)+2,
    ADC_33,  (3<<5)+3,
    ADC_34,  (3<<5)+4,
    ADC_35,  (3<<5)+5,
    ADC_36,  (3<<5)+6,
    ADC_37,  (3<<5)+7,
    ADC_38,  (3<<5)+8,
    ADC_39,  (3<<5)+9,
    ADC_40,  (4<<5)+0,
    ADC_41,  (4<<5)+1,
    ADC_42,  (4<<5)+2,
    ADC_43,  (4<<5)+3,
    ADC_44,  (4<<5)+4,
    ADC_MAX, (1<<5)+1,  // Ceiling, don't remove
#endif
};

#ifdef BATTCHECK_VpT
inline uint8_t battcheck() {
    // Return an composite int, number of "blinks", for approximate battery charge
    // (rounded down)
    // Uses the table above for return values
    // Return value is 3 bits of whole volts and 5 bits of tenths-of-a-volt
    VOLTAGE_TYPE voltage = get_voltage();
    uint8_t i = 2;
    // figure out how many times to blink
    // With V_CC as reference and 1.1V V_BG as input, ADC values are decreasing
    // as voltage increases
    #ifdef VCC_REF
    while (voltage < pgm_read_word(voltage_blinks + i))
        i += 2;
    return pgm_read_word(voltage_blinks + i - 1);
    #else
    while (voltage > pgm_read_byte(voltage_blinks + i))
        i += 2;
    return pgm_read_byte(voltage_blinks + i - 1);
    #endif
}
#else  // #ifdef BATTCHECK_VpT
inline uint8_t battcheck() {
    // Return an int, number of "blinks", for approximate battery charge
    // (rounded up)
    // Uses the table above for return values
    VOLTAGE_TYPE voltage = get_voltage();
    uint8_t i = 0;
    // figure out how many times to blink
    #ifdef VCC_REF
    while (voltage < pgm_read_word(voltage_blinks + i))
    #else
    while (voltage > pgm_read_byte(voltage_blinks + i))
    #endif
        i++;
    return i;
}
#endif  // BATTCHECK_VpT
#endif  // BATTCHECK


#endif  // TK_VOLTAGE_H
