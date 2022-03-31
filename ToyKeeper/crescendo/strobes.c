#ifndef STROBES_C
#define STROBES_C

#include "strobes.h"

#ifdef ANY_STROBE
void strobe(uint8_t ontime, uint8_t offtime) {
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

#endif
