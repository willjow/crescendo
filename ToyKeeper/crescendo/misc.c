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

void blink(uint8_t val, uint8_t on_4ms, uint8_t off_4ms) {
    if (!val) {
        // "zero" val gets a single short blink
        on_4ms = 2;
        val = 1;
    }

    for (; val>0; val--) {
        set_level(BLINK_BRIGHTNESS);
        _delay_4ms(on_4ms);
        set_level(0);
        _delay_4ms(off_4ms);
    }
}

void next_mode() {
    // allow an override, if it exists; mostly only useful for setting
    // RAMP_IDX, STEADY_IDX, or TURBO_IDX because the other mode indexes aren't
    // really known until compile time
    //
    // `mode_nums` is defined such that mode indexes should be
    // (mode num) - MAX_MODES, but ideally we don't rely on that
    if (next_mode_idx != DISABLE_MODE_OVERRIDE) {
        mode_idx = next_mode_idx;
        next_mode_idx = DISABLE_MODE_OVERRIDE;
        return;
    }

    mode_idx += 1;
    if (mode_idx >= sizeof(mode_cycle)) {
        // Wrap around
        // (wrap to steady mode (1), not ramp (0))
        mode_idx = STEADY_IDX;
    }
}

#endif
