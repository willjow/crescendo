#ifndef HWDEF_TRIPLEDOWN_H
#define HWDEF_TRIPLEDOWN_H

/* Tripledown driver layout
 *             --_--
 *     Reset -|1   8|- VCC
 *       OTC -|2   7|- Voltage ADC
 * PWM (FET) -|3   6|- PWM (6x7135)
 *       GND -|4   5|- PWM (1x7135)
 *             -----
 */

#define CAP_PIN     PB3     // pin 2, OTC
#define CAP_CHANNEL 0x03    // MUX 03 corresponds with PB3 (Star 4)
#define CAP_DIDR    ADC3D   // Digital input disable bit corresponding with PB3

#define PWM_PIN     PB1     // pin 6, 6x7135 PWM
#define PWM_LVL     OCR0B   // OCR0B is the output compare register for PB1
#define ALT_PWM_PIN PB0     // pin 5, 1x7135 PWM
#define ALT_PWM_LVL OCR0A   // OCR0A is the output compare register for PB0
#define FET_PWM_PIN PB4     // pin 3
#define FET_PWM_LVL OCR1B   // output compare register for PB4

#define ENABLE_PWM1B        // macro so we know to enable pwm on OCR1B

#define VOLTAGE_PIN PB2     // pin 7, voltage ADC
#define ADC_REF     0x80    // MUX[7:4] 1000 corresponds to 1.1V reference
#define ADC_CHANNEL 0x01    // MUX[3:0] 0001 corresponds with PB2
#define ADC_DIDR    ADC1D   // Digital input disable bit corresponding with PB2
#define ADC_PRSCL   0x06    // clk/64

//#define TEMP_DIDR   ADC4D
#define TEMP_CHANNEL 0b00001111

#define FAST 0xA3           // fast PWM both channels
#define PHASE 0xA1          // phase-correct PWM both channels

#define LAYOUT_DEFINED

#endif
