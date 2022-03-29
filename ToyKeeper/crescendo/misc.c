#ifndef MISC_C
#define MISC_C

#include "misc.h"

void _delay_500ms() {
    _delay_4ms(HALF_SECOND/4);
}

void blink(uint8_t val, uint8_t speed) {
    for (; val>0; val--)
    {
        set_level(BLINK_BRIGHTNESS);
        _delay_4ms(speed);
        set_level(0);
        _delay_4ms(speed);
        _delay_4ms(speed);
    }
}

void next_mode() {
    // allow an override, if it exists
    //if (next_mode_num < sizeof(modes)) {
    if (next_mode_num < 255) {
        mode_idx = next_mode_num;
        next_mode_num = 255;
        return;
    }

    mode_idx += 1;
    if (mode_idx >= sizeof(modes)) {
        // Wrap around
        // (wrap to steady mode (1), not ramp (0))
        mode_idx = 1;
    }
}

#endif
