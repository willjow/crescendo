// ATTINY: 25
#include "hwdef-H17Fx.h"

#define CALIBRATION_DEFINED
/********************** Voltage ADC calibration **************************/
// the ADC values we expect for specific voltages
#define ADC_44     278
#define ADC_43     284
#define ADC_42     290
#define ADC_41     297
#define ADC_40     304
#define ADC_39     311
#define ADC_38     318
#define ADC_37     326
#define ADC_36     335
#define ADC_35     344
#define ADC_34     353
#define ADC_33     363
#define ADC_32     374
#define ADC_31     385
#define ADC_30     397
#define ADC_29     410
#define ADC_28     424
#define ADC_27     439
#define ADC_26     455
#define ADC_25     473
#define ADC_24     491
#define ADC_23     512
#define ADC_22     534
#define ADC_21     558
#define ADC_20     585

/********************** Offtime capacitor calibration ********************/
#define CAP_SHORT_ONE_S 10    // ~1s
#define CAP_SHORT_HALF_S 45   // ~1/2s
#define CAP_SHORT_THIRD_S 90  // ~1/3s

#define CAP_SHORT CAP_SHORT_HALF_S


#define RAMP_DEFINED
/******** Ramp Config ********************************************************/
// 7x7135 + 1x7135
// ../../bin/level_calc.py 5.8 1 80 7135 8 0.25 127
// with
// ../../bin/level_calc.py 5.8 2 128 7135 8 0.25 127 7135 8 1.75 740
// 7x7135
#define RAMP_CH1 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8,10,13,15,17,20,22,25,28,31,34,37,40,43,47,50,54,58,62,66,70,75,79,84,89,94,99,104,110,115,121,127,134,140,147,154,161,168,176,184,192,200,208,217,226,236,245,255
// 1x7135
#define RAMP_CH2 8,8,8,8,8,8,9,9,9,9,9,9,10,10,10,10,11,11,12,12,12,13,13,14,15,15,16,17,17,18,19,20,21,22,24,25,26,28,29,31,33,35,37,39,41,43,46,48,51,54,57,60,64,68,71,75,80,84,89,93,99,104,110,115,122,128,135,142,149,157,165,173,182,191,201,211,221,232,243,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255
// Max 1x7135
#define CH0_MAX 80
