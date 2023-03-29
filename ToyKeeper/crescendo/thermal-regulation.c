#ifndef THERMAL_REGULATION_C
#define THERMAL_REGULATION_C

#include "thermal-regulation.h"

#define TEMP_ORIGIN 273  // roughly 0 C or 32 F (ish)
int16_t current_temperature() {
    ADC_on_temperature();
    get_temperature();  // throw this away
    // take multiple measurements; temperature is noisy
    uint16_t temperature = 0;
    uint8_t i;
    get_temperature();
    for (i = 0; i < 8; i++) {
        temperature += get_temperature();
        _delay_4ms(1);
    }

    // Right-adjusted temperature should be roughly 1 LSB per degree
    // Celsius. We can interpret the sum of 8 of these as Q12.3 fixed point (the
    // old LSB has effectively been shifted 3 bits over).
    //
    // Instead of shifting 3 bits back over to divide by 8, we can keep these
    // extra bits around for some more (noisy) precision.
    //
    // We want to make sure we still have headroom to do arithmetic with these
    // values, so we convert from Q12.3 fixed point to Q13.2 fixed point.
    temperature = (temperature >> 1) - (TEMP_ORIGIN << 2);
    return temperature;
}

inline void monitor_temperature(
    uint8_t mode,
    uint8_t *temperatures,
    uint8_t *overheat_count,
    uint8_t *underheat_count,
    uint8_t first_temp_reading,
    uint8_t first_loop,
    uint8_t *loop_count
) {
    // How far ahead should we predict?
    // This will be applied as a left bit shift for instruction efficiency, in Q13.2
    // space. The effective difference in Celsius would be
    // 2^{THERM_PREDICTION_STRENGTH - 2}
    #define THERM_PREDICTION_STRENGTH 4

    // How proportional should the adjustments be?
    // As with `THERM_PREDICTION_STRENGTH`, this will be applied as a right bit
    // shift in Q13.2 space.
    #define THERM_DIFF_ATTENUATION 4

    // How low is the lowpass filter?
    #define THERM_LOWPASS 8

    // Lowest ramp level; never go below this (sanity check)
    #define THERM_FLOOR (MAX_LEVEL / 4)

    // Highest temperature allowed
    // (convert configured Celsius value to Q13.2 fixed-point)
    #define THERM_CEIL (therm_ceil << 2)

    // Acceptable temperature window size in Celsius
    #define THERM_WINDOW_SIZE 8

    // Number of history steps
    #define THERM_HISTORY_SIZE 8

    // Make sure to use the appropriate level variable
    uint8_t *current_level = (mode == STEADY_E) ? &ramp_level : &actual_level;

    int16_t temperature = current_temperature();
    int16_t projected;  // Fight the future!
    int16_t diff;

    // initial setup, only once
    if (first_temp_reading) {
        for (uint8_t t = 0; t < THERM_HISTORY_SIZE; t++)
            temperatures[t] = temperature;
    }

    // rotate measurements and add a new one
    for (uint8_t t = 0; t < (THERM_HISTORY_SIZE - 1); t++) {
        temperatures[t] = temperatures[t + 1];
    }
    temperatures[THERM_HISTORY_SIZE - 1] = temperature;

    // guess what the temp will be `THERM_HISTORY_SIZE` steps in the future if
    // it continues to increase at roughly the same rate
    diff = temperature - temperatures[0];
    projected = temperature + (diff << THERM_PREDICTION_STRENGTH);

    if (0) {} // placeholder

    #ifdef THERM_CALIBRATION_MODE
    // never step down in thermal calibration mode
    else if (mode == THERM_CALIBRATION_MODE_E) {
        if (first_loop) {
            // Blink out the current temperature limit.
            //
            // The user should just set/check this once and then hard code the
            // value into `DEFAULT_THERM_CEIL` and disable thermal calibration
            // mode to save some program space.
            _delay_input();
            blink_num(therm_ceil);
            buzz();  // opportunity to cancel before doing anything

            therm_ceil = DEFAULT_THERM_CEIL;
            set_level(MAX_LEVEL / 4);
            save_state();
            _delay_s();
            _delay_s();
            // Turn power up all the way for calibration purposes
            set_level(MAX_LEVEL);
        }
        // Use the current temperature as the new ceiling value; making sure to
        // convert from Q13.2 back to Celsius.
        // (Also, use less aggressive prediction.)
        therm_ceil = (temperature + (diff << (THERM_PREDICTION_STRENGTH - 1))) >> 2;

        // Save state periodically (but not too often)
        if (*loop_count > 3)
        {
            *loop_count = 0;
            save_state();
        }
        // Don't repeat for a little while
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
            uint8_t stepdown = *current_level - exceed;
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
            if (*current_level > THERM_FLOOR) {
                // ramp to stepdown level
                ramp(current_level, stepdown);
            }
        }
        else {
            (*overheat_count)++;
        }
    }
    else {  // not too hot
        *overheat_count = 0;  // we're definitely not too hot
        // too cold?  step back up?
        if (projected < (THERM_CEIL - (THERM_WINDOW_SIZE << 2))) {
            if (*underheat_count > (THERM_LOWPASS / 2)) {
                *underheat_count = 0;
                // never go above the user's requested level
                if (*current_level < target_level) {
                    // step up slowly
                    ramp(current_level, *current_level + 1);
                }
            } else {
                (*underheat_count)++;
            }
        }
    }
}

#endif
