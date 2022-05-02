#ifndef THERMAL_REGULATION_H
#define THERMAL_REGULATION_H

uint8_t target_level;  // ramp level before thermal stepdown
uint8_t therm_ceil = DEFAULT_THERM_CEIL;

int16_t current_temperature();
inline void monitor_temperature(uint8_t mode,
                                uint8_t *temperatures,
                                uint8_t *overheat_count,
                                uint8_t *underheat_count,
                                uint8_t first_temp_reading,
                                uint8_t first_loop, uint8_t *loop_count);

#endif
