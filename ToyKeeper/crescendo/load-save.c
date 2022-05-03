#ifndef LOAD_SAVE_C
#define LOAD_SAVE_C

#include "load-save.h"

#ifdef MEMORY_WL
void save_state_wl() {  // save the current level/mode (with wear leveling)
    #ifdef MEMTOGGLE
    // only save when memory is enabled
    if (memory)
    #endif
    {
        for (uint8_t i = 0; i < EEP_WL_OPTIONS_END; i++) {
            eeprom_write_byte((uint8_t *)eepos, 0xff);  // erase old state
            eepos++;
        }
        if (eepos > EEP_WL_END)
            eepos = EEP_WL_START;
        #ifdef RAMP_MEMORY
        // save current brightness
        eeprom_write_byte((uint8_t *)(eepos + EEP_WL_RAMP_LEVEL), ramp_level);
        #endif
        #ifdef MODE_MEMORY
        // save current mode
        eeprom_write_byte((uint8_t *)(eepos + EEP_WL_MODE_ID), mode_id);
        #endif
    }
}
#endif

#ifdef CONFIG_MODE
void save_state() {
    #ifdef MEMORY_WL
    save_state_wl();
    #endif
    #ifdef MEMTOGGLE
    eeprom_write_byte((uint8_t *)EEP_MEMORY, memory);
    #endif
    #ifdef THERM_CALIBRATION_MODE
    eeprom_write_byte((uint8_t *)EEP_THERM_CEIL, therm_ceil);
    #endif
}
#endif

#if defined(CONFIG_MODE) || defined(MEMORY_WL)
void restore_state() {
    uint8_t eep;
    #ifdef MEMTOGGLE
    // memory is either 1 or 0
    // (if it's unconfigured, 0xFF, assume it's off)
    eep = eeprom_read_byte((uint8_t *)EEP_MEMORY);
    if (eep < 2) { memory = eep; }
    else { memory = 0; }
    #endif

    #ifdef THERM_CALIBRATION_MODE
    // load therm_ceil
    eep = eeprom_read_byte((uint8_t *)EEP_THERM_CEIL);
    if (eep > 0) {
        therm_ceil = eep;
    }
    #endif

    #ifdef MEMORY_WL
    // find the memorized level/mode
    for (eepos = EEP_WL_START; eepos <= EEP_WL_END; eepos += EEP_WL_OPTIONS_END) {
        eep = eeprom_read_byte((const uint8_t *)eepos);
        if (eep != 0xff) {
            #ifdef RAMP_MEMORY
            saved_ramp_level = eeprom_read_byte((const uint8_t *)(eepos + EEP_WL_RAMP_LEVEL));
            #endif
            #ifdef MODE_MEMORY
            saved_mode_id = eeprom_read_byte((const uint8_t *)(eepos + EEP_WL_MODE_ID));
            #endif
            break;
        }
    }
    #endif
}
#endif  // ifdef CONFIG_MODE

#endif
