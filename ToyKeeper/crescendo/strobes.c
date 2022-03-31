#ifndef STROBES_C
#define STROBES_C

#include "strobes.h"

#ifdef ANY_STROBE
#ifdef POLICE_STROBE
void strobe(uint8_t ontime, uint8_t offtime) {
#else
inline void strobe(uint8_t ontime, uint8_t offtime) {
#endif
    uint8_t i;
    for(i=0;i<8;i++) {
        set_level(RAMP_SIZE);
        _delay_4ms(ontime);
        set_level(0);
        _delay_4ms(offtime);
    }
}
#endif

#ifdef PARTY_STROBES
void party_strobe(uint8_t ontime, uint8_t offtime) {
    set_level(RAMP_SIZE);
    if (ontime) {
        _delay_ms(ontime);
    } else {
        _delay_zero();
    }
    set_level(0);
    _delay_ms(offtime);
}

void party_strobe_loop(uint8_t ontime, uint8_t offtime) {
    uint8_t i;
    for(i=0;i<32;i++) {
        party_strobe(ontime, offtime);
    }
}
#endif

#ifdef SOS
void SOS_mode() {
#define DIT_LENGTH (200 / 4)
#define DAH_LENGTH (DIT_LENGTH * 3)
    blink(3, DIT_LENGTH, DIT_LENGTH);
    blink(3, DAH_LENGTH, DIT_LENGTH);
    blink(3, DIT_LENGTH, DIT_LENGTH);
    _delay_s(); _delay_s();
}
#endif

#ifdef BIKING_MODE
void biking_mode(uint8_t lo, uint8_t hi) {
    #ifdef FULL_BIKING_MODE
    // normal version
    uint8_t i;
    for(i=0;i<4;i++) {
        //set_output(255,0);
        set_mode(hi);
        _delay_4ms(2);
        //set_output(0,255);
        set_mode(lo);
        _delay_4ms(15);
    }
    //_delay_ms(720);
    _delay_s();
    #else  // smaller bike mode
    // small/minimal version
    set_mode(hi);
    //set_output(255,0);
    _delay_4ms(4);
    set_mode(lo);
    //set_output(0,255);
    _delay_s();
    #endif  // ifdef FULL_BIKING_MODE
}
#endif

#endif
