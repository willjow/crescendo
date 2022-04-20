#ifndef MEMORY_C
#define MEMORY_C

#include "memory.h"

void memory_override() {
    // moon mode for a bit
    set_level(1);
    // if the user taps quickly, go to the real steady mode
    next_mode_id = STEADY_IDX;

    _delay_input();

    // if they didn't tap quickly, go to the memorized level/mode
    #if defined(MANUAL_LEVEL)
    ramp_level = MANUAL_LEVEL;
    #elif defined(RAMP_MEMORY)
    ramp_level = saved_ramp_level;
    #endif

    #ifdef MODE_MEMORY
    mode_id = saved_mode_id;
    #else
    mode_id = STEADY_IDX;
    #endif

    #ifdef MEMORY_WL
    // remember for next time
    save_state_wl();
    #endif
}

#endif
