#ifndef HEART_BEACON_MODE_C
#define HEART_BEACON_MODE_C

#include "heart-beacon-mode.h"

void heart_beacon_mode() {
    set_level(MAX_LEVEL);
    _delay_4ms(1);
    set_level(0);
    _delay_4ms(250/4);
    set_level(MAX_LEVEL);
    _delay_4ms(1);
    set_level(0);
    _delay_4ms(750/4);
}

#endif
