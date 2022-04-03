#ifndef BIKING_MODE_C
#define BIKING_MODE_C

#include "biking-mode.h"

void biking_mode(uint8_t lo, uint8_t hi) {
    #ifdef FULL_BIKING_MODE
    // normal version
    uint8_t i;
    for(i=0;i<4;i++) {
        //set_output(255,0);
        set_level(hi);
        _delay_4ms(2);
        //set_output(0,255);
        set_level(lo);
        _delay_4ms(15);
    }
    //_delay_ms(720);
    _delay_s();
    #else  // smaller bike mode
    // small/minimal version
    set_level(hi);
    //set_output(255,0);
    _delay_4ms(4);
    set_level(lo);
    //set_output(0,255);
    _delay_s();
    #endif  // ifdef FULL_BIKING_MODE
}

#endif
