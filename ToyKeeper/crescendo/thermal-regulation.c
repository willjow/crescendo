#ifndef THERMAL_REGULATION_C
#define THERMAL_REGULATION_C

#include "thermal-regulation.h"

#ifdef THERMAL_REGULATION
#define TEMP_ORIGIN 275  // roughly 0 C or 32 F (ish)
int16_t current_temperature() {
    ADC_on_temperature();
    // average a few values; temperature is noisy
    // (use some of the noise as extra precision, ish)
    uint16_t temp = 0;
    uint8_t i;
    get_temperature();
    for(i=0; i<8; i++) {
        temp += get_temperature();
        _delay_4ms(1);
    }
    // convert 12.3 fixed-point to 13.2 fixed-point
    // ... and center it at 0 C
    temp = (temp>>1) - (TEMP_ORIGIN<<2);
    return temp;
}
#endif  // ifdef THERMAL_REGULATION

#define THERM_HISTORY_SIZE 8
inline void monitor_temperature(uint8_t mode,
                                uint8_t *temperatures,
                                uint8_t *overheat_count,
                                uint8_t *underheat_count,
                                uint8_t first_temp_reading,
                                uint8_t first_loop, uint8_t *loop_count) {
    // how far ahead should we predict?
    #define THERM_PREDICTION_STRENGTH 4
    // how proportional should the adjustments be?
    #define THERM_DIFF_ATTENUATION 4
    // how low is the lowpass filter?
    #define THERM_LOWPASS 8
    // lowest ramp level; never go below this (sanity check)
    #define THERM_FLOOR (RAMP_SIZE/4)
    // highest temperature allowed
    // (convert configured value to 13.2 fixed-point)
    #define THERM_CEIL (therm_ceil<<2)
    // acceptable temperature window size in C
    #define THERM_WINDOW_SIZE 8

    int16_t temperature = current_temperature();
    int16_t projected;  // Fight the future!
    int16_t diff;

    // initial setup, only once
    if (first_temp_reading) {
        for (uint8_t t=0; t<THERM_HISTORY_SIZE; t++)
            temperatures[t] = temperature;
    }

    // rotate measurements and add a new one
    for(uint8_t t=0; t<THERM_HISTORY_SIZE-1; t++) {
        temperatures[t] = temperatures[t+1];
    }
    temperatures[THERM_HISTORY_SIZE-1] = temperature;

    // guess what the temp will be several seconds in the future
    diff = temperature - temperatures[0];
    projected = temperature + (diff<<THERM_PREDICTION_STRENGTH);

    if (0) {} // placeholder

    #ifdef THERM_CALIBRATION_MODE
    // never step down in thermal calibration mode
    else if (mode == THERM_CALIBRATION_MODE) {
        if (first_loop) {
            // TODO: blink out current temperature limit
            // let user set default or max limit?
            therm_ceil = DEFAULT_THERM_CEIL;
            set_mode(RAMP_SIZE/4);
            save_state();
            _delay_s();
            _delay_s();
            // turn power up all the way for calibration purposes
            set_mode(RAMP_SIZE);
        }
        // use the current temperature as the new ceiling value
        //tempCeil = projected >> 2;
        // less aggressive prediction
        therm_ceil = (temperature + (diff<<(THERM_PREDICTION_STRENGTH-1))) >> 2;
        // Don't let user exceed maximum limit
        if (therm_ceil > MAX_THERM_CEIL) {
            therm_ceil = MAX_THERM_CEIL;
        }
        // save state periodically (but not too often)
        if (*loop_count > 3)
        {
            *loop_count = 0;
            save_state();
        }
        // don't repeat for a little while
        _delay_input();
    }
    #endif

    // too hot, step down (maybe)
    else if (projected >= THERM_CEIL) {
        *underheat_count = 0;  // we're definitely not too cold
        if (*overheat_count > THERM_LOWPASS) {
            *overheat_count = 0;

            // how far above the ceiling?
            int16_t exceed = (projected - THERM_CEIL) >> THERM_DIFF_ATTENUATION;
            if (exceed < 1) { exceed = 1; }
            uint8_t stepdown = actual_level - exceed;
            // never go under the floor; zombies in the cellar
            if (stepdown < THERM_FLOOR) {
                stepdown = THERM_FLOOR;
            }
            // avoid a bug: stepping "down" from moon to THERM_FLOOR
            // if user turned the light down during lowpass period
            if (stepdown > target_level) {
                stepdown = target_level;
            }
            // really, don't try to regulate below the floor
            if (actual_level > THERM_FLOOR) {
                set_mode(stepdown);
            }
        }
        else {
            (*overheat_count)++;
        }
    }
    else {  // not too hot
        *overheat_count = 0;  // we're definitely not too hot
        // too cold?  step back up?
        if (projected < (THERM_CEIL - (THERM_WINDOW_SIZE<<2))) {
            if (*underheat_count > (THERM_LOWPASS/2)) {
                *underheat_count = 0;
                // never go above the user's requested level
                if (actual_level < target_level) {
                    set_mode(actual_level + 1);  // step up slowly
                }
            } else {
                (*underheat_count)++;
            }
        }
    }
}

#endif
