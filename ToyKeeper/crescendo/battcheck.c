#ifndef BATTCHECK_C
#define BATTCHECK_C

void battcheck_mode() {
    #ifdef BATTCHECK_VpT
    // blink out volts and tenths
    uint8_t result = battcheck();
    uint8_t volts = result >> 5;
    uint8_t tenths = result & 0b00011111;
    blink(volts, BLINK_ONTIME, BLINK_OFFTIME);
    _delay_4ms(BLINK_SPACE);
    blink(tenths, BLINK_ONTIME, BLINK_OFFTIME);
    #else  // ifdef BATTCHECK_VpT
    // blink zero to five times to show voltage
    // (or zero to nine times, if 8-bar mode)
    // (~0%, ~25%, ~50%, ~75%, ~100%, >100%)
    blink(battcheck(), BLINK_ONTIME, BLINK_OFFTIME);
    #endif  // ifdef BATTCHECK_VpT
    // wait between readouts
    _delay_s();
}

void monitor_voltage(uint8_t mode, uint8_t *lowbatt_cnt) {
    //if (ADCSRA & (1 << ADIF)) {  // if a voltage reading is ready
    {  // nope, always execute
        //uint8_t voltage = ADCH;  // get the waiting value
        uint8_t voltage = get_voltage();  // get a new value, first is unreliable
        // See if voltage is lower than what we were looking for
        if (voltage < ADC_LOW) {
            (*lowbatt_cnt)++;
        } else {
            *lowbatt_cnt = 0;
        }
        // See if it's been low for a while, and maybe step down
        if (*lowbatt_cnt >= 8) {
            // DEBUG: blink on step-down:
            //set_level(0);  _delay_ms(100);

            if (mode != STEADY_E) {
                // step "down" from special modes to medium-low
                mode_idx = STEADY_IDX;
                ramp_level = MAX_LEVEL/4;
            }
            else {
                if (ramp_level > 1) {  // solid non-moon mode
                    // drop by 50% each time
                    ramp_level = (actual_level >> 1);
                } else { // Already at the lowest mode
                    // Turn off the light
                    poweroff();
                }
            }
            set_level(ramp_level);
            target_level = ramp_level;
            //save_state_wl();  // we didn't actually change the mode
            *lowbatt_cnt = 0;
            // Wait before lowering the level again
            _delay_s();
        }

        // Make sure conversion is running for next time through
        // (not relevant with thermal regulation also active)
        //ADCSRA |= (1 << ADSC);
    }
}

#endif
