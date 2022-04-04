#ifndef BIKING_MODE_C
#define BIKING_MODE_C

#include "biking-mode.h"

void biking_mode(uint8_t steady_level) {
    uint8_t burst_level = steady_level << 1;
    if (steady_level > (MAX_LEVEL / 2))
        burst_level = MAX_LEVEL;
    if (steady_level > MAX_BIKING_LEVEL)
        steady_level = MAX_BIKING_LEVEL;
    #ifdef FULL_BIKING_MODE
    // normal version
    for (uint8_t i = 0; i < 4; i++) {
        set_level(burst_level);
        _delay_4ms(2);
        set_level(steady_level);
        _delay_4ms(15);
    }
    #else  // smaller bike mode
    // small/minimal version
    set_level(burst_level);
    _delay_4ms(4);
    set_level(steady_level);
    #endif  // ifdef FULL_BIKING_MODE
    //_delay_ms(720);
    _delay_s();
}

#endif
