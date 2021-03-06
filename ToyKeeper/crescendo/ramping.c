#ifndef RAMPING_C
#define RAMPING_C

#include "ramping.h"

#ifdef RAMP_CH3
void set_output(uint8_t pwm1, uint8_t pwm2, uint8_t pwm3) {
#else
#ifdef RAMP_CH2
void set_output(uint8_t pwm1, uint8_t pwm2) {
#else
void set_output(uint8_t pwm1) {
#endif
#endif
    PWM_LVL = pwm1;
    #ifdef RAMP_CH2
    ALT_PWM_LVL = pwm2;
    #endif
    #ifdef RAMP_CH3
    FET_PWM_LVL = pwm3;
    #endif
}

void set_level(uint8_t level) {
    actual_level = level;
    TCCR0A = PHASE;
    if (level == 0) {
        #ifdef RAMP_CH3
        set_output(0,0,0);
        #else
        #ifdef RAMP_CH2
        set_output(0,0);
        #else
        set_output(0);
        #endif  // ifdef RAMP_CH2
        #endif  // ifdef RAMP_CH3
    } else {
        /*
        if (level > 2) {
            // divide PWM speed by 2 for lowest modes,
            // to make them more stable
            TCCR0A = FAST;
        }
        */
        #ifdef RAMP_CH3
        set_output(pgm_read_byte(ramp_ch1 + level - 1),
                   pgm_read_byte(ramp_ch2 + level - 1),
                   pgm_read_byte(ramp_ch3 + level - 1));
        #else
        #ifdef RAMP_CH2
        set_output(pgm_read_byte(ramp_ch1 + level - 1),
                   pgm_read_byte(ramp_ch2 + level - 1));
        #else
        set_output(pgm_read_byte(ramp_ch1 + level - 1));
        #endif
        #endif
    }
}

void ramp_mode() {
    set_level(ramp_level);  // turn light on

    // double-tap to ramp down
    if (fast_presses == 1) {
        next_mode_id = RAMP_IDX;    // stay in ramping mode
        ramp_dir = -1;              // prepare to ramp down
    }
    // triple-tap to enter turbo
    else if (fast_presses == 2) {
        next_mode_id = TURBO_IDX;   // bypass "steady" mode
    }

    // wait a bit before actually ramping
    // (give the user a chance to select moon, or double-tap)
    _delay_input();

    // if we got through the delay, assume normal operation
    // (not trying to double-tap or triple-tap)
    // (next mode should be normal)
    next_mode_id = DISABLE_MODE_OVERRIDE;
    // ramp up on single tap
    // (cancel earlier preemptive reversal)
    if (fast_presses == 1) {
        ramp_dir = 1;
    }
    // don't want this confusing us any more
    fast_presses = 0;

    // Just in case (SRAM could have partially decayed)
    //ramp_dir = (ramp_dir == 1) ? 1 : -1;
    // Do the actual ramp
    while (1) {
        set_level(ramp_level);
        _delay_4ms(RAMP_TIME/RAMP_SIZE/4);
        if (
            ((ramp_dir > 0) && (ramp_level >= MAX_LEVEL))
            ||
            ((ramp_dir < 0) && (ramp_level <= 1))
            )
            break;
        ramp_level += ramp_dir;
    }
    #ifdef STOP_AT_ENDS
    // go to steady mode; not the first loop anymore so we need to manually set
    // the appropriate variables
    mode_id = STEADY_IDX;
    next_mode_id = RAMP_IDX;
    #ifdef THERMAL_REGULATION
    target_level = ramp_level;
    #endif
    #endif

    #ifdef BLINK_AT_TOP
    if (ramp_dir == 1) {
        set_level(0);
        _delay_4ms(2);
        set_level(ramp_level);
    }
    #endif
    ramp_dir = -ramp_dir;
}

void steady_mode() {
    // normal flashlight mode
    set_level(ramp_level);
    #ifdef THERMAL_REGULATION
    target_level = ramp_level;
    #endif

    // Wait for user to tap again to advance to the next mode
    //next_mode_id = DISABLE_MODE_OVERRIDE;
    _delay_input();
    // After a delay, assume user wants to adjust ramp
    // instead of going to next mode (unless they're
    // tapping rapidly, in which case we should advance to turbo)
    next_mode_id = RAMP_IDX;
}

void turbo_mode() {
    // turbo is special because it's easier to handle that way
    set_level(MAX_LEVEL);
    #ifdef THERMAL_REGULATION
    target_level = MAX_LEVEL;
    #endif

    // Wait for user to tap again to advance to the next mode
    //next_mode_id = DISABLE_MODE_OVERRIDE;
    _delay_input();
    // go back to the previously-memorized level
    // if the user taps after a delay,
    // instead of advancing to blinkies
    // (allows something similar to "momentary" turbo)
    next_mode_id = STEADY_IDX;
}

#endif
