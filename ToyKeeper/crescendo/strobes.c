#ifndef STROBES_C
#define STROBES_C

#include "strobes.h"

#ifdef TACTICAL_STROBES
void strobe(uint8_t ontime, uint8_t offtime) {
    uint8_t i;
    for(i=0;i<8;i++) {
        set_level(MAX_LEVEL);
        _delay_4ms(ontime);
        set_level(0);
        _delay_4ms(offtime);
    }
}

void tactical_strobe() {
    // 10Hz tactical strobe
    strobe(33/4,67/4);
}

void police_strobe() {
    // police-like strobe
    strobe(20/4,40/4);
    strobe(40/4,80/4);
}
#endif

#ifdef RANDOM_STROBE
void random_strobe() {
    // pseudo-random strobe
    uint8_t ms = (34 + (pgm_rand() & 0x3f))>>2;
    //strobe(ms, ms);
    set_level(MAX_LEVEL);
    _delay_4ms(ms);
    set_level(0);
    _delay_4ms(ms);
    //strobe(ms, ms);
}
#endif

#ifdef PARTY_STROBES
void party_strobe(uint8_t ontime, uint8_t offtime) {
    set_level(MAX_LEVEL);
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

void party_strobe_12() {
    party_strobe_loop(1,79);
}

void party_strobe_24() {
    party_strobe_loop(0,41);
}

void party_strobe_60() {
    party_strobe_loop(0,15);
}

void party_varstrobe_1() {
    uint8_t j, speed;
    for(j=0; j<66; j++) {
        if (j<33) { speed = j; }
        else { speed = 66-j; }
        party_strobe(1,(speed+33-6)<<1);
    }
}

void party_varstrobe_2() {
    uint8_t j, speed;
    for(j=0; j<100; j++) {
        if (j<50) { speed = j; }
        else { speed = 100-j; }
        party_strobe(0, speed+9);
    }
}
#endif

#endif
