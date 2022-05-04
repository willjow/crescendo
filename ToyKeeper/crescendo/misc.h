#ifndef MISC_H
#define MISC_H

#if defined(VOLTAGE_MON) || defined(GOODNIGHT)
void poweroff();
#endif
void _delay_input();
void blink(uint8_t val, uint8_t on_4ms, uint8_t off_4ms, uint8_t level);
#define buzz() blink(32, 500 / 32 / 4, 500 / 32 / 2, BLINK_BRIGHTNESS);
#ifdef USE_BLINK_NUM
void blink_num(uint16_t num);
#endif
void next_mode();

#endif
