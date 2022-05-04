#ifndef SOS_MODE_C
#define SOS_MODE_C

#include "sos-mode.h"

void SOS_mode(uint8_t level) {
    blink(3, DIT_LENGTH, DIT_LENGTH, level);
    blink(3, DAH_LENGTH, DIT_LENGTH, level);
    blink(3, DIT_LENGTH, DIT_LENGTH, level);
    _delay_s(); _delay_s();
}

#endif
