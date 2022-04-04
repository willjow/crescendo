#ifndef CONFIG_MODE_C
#define CONFIG_MODE_C

#include "config-mode.c"

void toggle(uint8_t *var, uint8_t num) {
    // Used for config mode
    // Changes the value of a config option, waits for the user to "save"
    // by turning the light off, then changes the value back in case they
    // didn't save.  Can be used repeatedly on different options, allowing
    // the user to change and save only one at a time.
    blink(num, BLINK_SPEED / 16, BLINK_SPEED / 8);  // indicate which option number this is
    _delay_4ms(250/4);
    *var ^= 1;
    save_state();
    // "buzz" for a while to indicate the active toggle window
    blink(32, 500 / 32 / 4, 500 / 32 / 2);
    // if the user didn't click, reset the value and return
    *var ^= 1;
    save_state();
    _delay_s();
}

void config_mode(uint8_t *dummy) {
    _delay_s();       // wait for user to stop fast-pressing button
    fast_presses = 0; // exit this mode after one use
                      //mode = STEADY_E;
    mode_idx = STEADY_IDX;
    next_mode_num = DISABLE_MODE_OVERRIDE;

    uint8_t t = 0;

    #ifdef MEMTOGGLE
    // turn memory on/off
    // (click during the "buzz" to change the setting)
    toggle(&memory, ++t);
    #endif  // ifdef MEMTOGGLE

    #ifdef THERM_CALIBRATION_MODE
    // Enter temperature calibration mode?
    next_mode_num = THERM_CALIBRATION_MODE_E;
    toggle(dummy, ++t);  // doesn't actually set anything
    mode_idx = STEADY_IDX;
    next_mode_num = DISABLE_MODE_OVERRIDE;
    #endif
}

#endif
