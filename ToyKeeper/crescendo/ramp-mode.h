#ifndef RAMP_MODE_H
#define RAMP_MODE_H

#ifdef RAMP_CH3
void set_output(uint8_t pwm1, uint8_t pwm2, uint8_t pwm3);
#else
#ifdef RAMP_CH2
void set_output(uint8_t pwm1, uint8_t pwm2);
#else
void set_output(uint8_t pwm1);
#endif
#endif

void set_level(uint8_t level);
#define set_mode set_level

void ramp_mode();

#endif
