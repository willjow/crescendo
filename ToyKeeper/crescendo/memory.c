#ifndef MEMORY_C
#define MEMORY_C

#include "memory.h"

void memory_override() {
    // moon mode for a bit
    set_level(1);
    // if the user taps quickly, go to the real steady mode
    next_mode_idx = STEADY_IDX;

    _delay_input();

    // if they didn't tap quickly, go to the memorized level/mode
    #ifdef RAMP_MEMORY
    ramp_level = saved_ramp_level;
    #endif
    #ifdef MODE_MEMORY
    mode_idx = saved_mode_idx;
    #endif
    // remember for next time
    save_state_wl();
}

#endif
