#ifndef GOODNIGHT_MODE_C
#define GOODNIGHT_MODE_C

#include "goodnight_mode.h"

void goodnight_mode() {
    uint8_t i, j;
    // signal that this is *not* the STEADY mode
    blink(2, BLINK_SPEED / 32, BLINK_SPEED / 16);
    #define GOODNIGHT_TOP (RAMP_SIZE/6)
    // ramp up instead of going directly to the top level
    // (probably pointless in this UI)
    /*
    for (i=1; i<=GOODNIGHT_TOP; i++) {
        set_mode(i);
        _delay_4ms(2*RAMP_TIME/RAMP_SIZE/4);
    }
    */
    // ramp down over about an hour
    for(i=GOODNIGHT_TOP; i>=1; i--) {
        set_mode(i);
        // how long the down ramp should last, in seconds
        #define GOODNIGHT_TIME 60*60
        // how long does _delay_s() actually last, in seconds?
        // (calibrate this per driver, probably)
        #define ONE_SECOND 1.03
        #define GOODNIGHT_STEPS (1+GOODNIGHT_TOP)
        #define GOODNIGHT_LOOPS (uint8_t)((GOODNIGHT_TIME) / ((2*ONE_SECOND) * GOODNIGHT_STEPS))
        // NUM_LOOPS = (60*60) / ((2*ONE_SECOND) * (1+MODE_LOW-MODE_MOON))
        // (where ONE_SECOND is how many seconds _delay_s() actually lasts)
        // (in my case it's about 0.89)
        for(j=0; j<GOODNIGHT_LOOPS; j++) {
            _delay_s();
            _delay_s();
            //_delay_ms(10);
        }
    }
    poweroff();
}

#endif
