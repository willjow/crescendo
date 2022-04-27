#ifndef HWDEF_NANJG_H
#define HWDEF_NANJG_H

/* NANJG driver layout
 */
#define STAR2_PIN   PB0
#define STAR3_PIN   PB4
#define STAR4_PIN   PB3
#define PWM_PIN     PB1
#define VOLTAGE_PIN PB2
#define ADC_REF     0x80    // MUX[7:4] 1000 corresponds to 1.1V reference
#define ADC_CHANNEL 0x01    // MUX[3:0] 0001 corresponds with PB2
#define ADC_DIDR    ADC1D   // Digital input disable bit corresponding with PB2
#define ADC_PRSCL   0x06    // clk/64

#define PWM_LVL     OCR0B   // OCR0B is the output compare register for PB1

#define FAST 0x23           // fast PWM channel 1 only
#define PHASE 0x21          // phase-correct PWM channel 1 only

#define LAYOUT_DEFINED

#endif
