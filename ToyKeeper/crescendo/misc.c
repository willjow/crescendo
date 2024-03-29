#ifndef MISC_C
#define MISC_C

#include "misc.h"

#if defined(VOLTAGE_MON) || defined(GOODNIGHT)
void poweroff() {
    // Turn off main LED
    set_level(0);
    // Power down as many components as possible
    ADCSRA &= ~(1<<7); //ADC off
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    sleep_mode();
}
#endif

void _delay_input() {
    _delay_4ms(INPUT_WAIT_TIME / 4);
}

void blink(uint8_t val, uint8_t on_4ms, uint8_t off_4ms, uint8_t level) {
    if (!val) {
        // "zero" val gets a single short blink
        on_4ms = 2;
        val = 1;
    }

    for (; val>0; val--) {
        set_level(level);
        _delay_4ms(on_4ms);
        set_level(0);
        _delay_4ms(off_4ms);
    }
}

#ifdef USE_BLINK_NUM
void blink_num(uint16_t num) {
    // we can keep this around because we only ever access relevant indexes
    static uint8_t digits[5];
    uint8_t i = 5;
    while (num) {
        digits[i - 1] = num % 10;
        num /= 10;
        i--;
    }
    while (i < 5) {
        blink(digits[i], BLINK_ONTIME, BLINK_OFFTIME, BLINK_BRIGHTNESS);
        _delay_4ms(BLINK_SPACE);
        i++;
    }
}
#endif

void delay_set_next_mode(uint8_t mode_id) {
    // During this delay period, a fast press should go to the next mode
    _delay_input();
    // After this delay, a fast press should go to ramp
    next_mode_id = mode_id;
}

void next_mode() {
    // allow an override, if it exists; mostly only useful for setting
    // RAMP_IDX, STEADY_IDX, or TURBO_IDX because the other mode indexes aren't
    // really known until compile time
    //
    // `mode_num` is defined such that mode indexes should be
    // (mode num) - MAX_MODES, but ideally we don't rely on that
    //
    // ------------------------
    //
    // On the other hand, we do
    //
    //      if (mode_id < sizeof(mode_cycle))
    //          mode = mode_cycle[mode_id];
    //      else
    //          mode = mode_id;
    //
    // at the beginning of the main loop, so we can still override arbitrarily
    // by setting `mode_id` to an `enum mode_num` instead of an index of
    // `mode_cycle[]`.
    if (next_mode_id == NEXT_MODE_IN_CYCLE) {
        mode_id += 1;
        if (mode_id >= mode_cycle_length) {
            // Wrap around to steady mode, not ramp
            mode_id = STEADY_IDX;
        }
    } else {
        mode_id = next_mode_id;
        next_mode_id = NEXT_MODE_IN_CYCLE;
    }
}

#endif
