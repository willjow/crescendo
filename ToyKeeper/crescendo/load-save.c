#ifndef LOAD_SAVE_C
#define LOAD_SAVE_C

#include "load-save.h"

#ifdef MEMORY
void save_mode() {  // save the current mode index (with wear leveling)
    #ifdef MEMTOGGLE
    // only save when memory is enabled
    if (memory)
    #endif
    {
        eeprom_write_byte((uint8_t *)(eepos), 0xff);     // erase old state
        eeprom_write_byte((uint8_t *)(++eepos), 0xff);     // erase old state
        eepos++;
        if (eepos > EEP_WL_END)
            eepos = EEP_WL_START;
        // save current mode
        eeprom_write_byte((uint8_t *)(eepos), mode_idx);
        // save current brightness
        eeprom_write_byte((uint8_t *)(eepos + 1), ramp_level);
    }
}
#endif

#ifdef CONFIG_MODE
void save_state() {
    #ifdef MEMORY
    save_mode();
    #endif
    #ifdef MEMTOGGLE
    eeprom_write_byte((uint8_t *)EEP_MEMORY, memory);
    #endif
    #ifdef THERM_CALIBRATION_MODE
    eeprom_write_byte((uint8_t *)EEP_THERM_CEIL, therm_ceil);
    #endif
}
#endif

#if defined(MEMORY) || defined(CONFIG_MODE)
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
    if ((eep > 0) && (eep < MAX_THERM_CEIL)) {
        therm_ceil = eep;
    }
    #endif

    #ifdef MEMORY
    // find the mode index and last brightness level
    for (eepos = EEP_WL_START; eepos <= EEP_WL_END; eepos += 2) {
        eep = eeprom_read_byte((const uint8_t *)eepos);
        if (eep != 0xff) {
            saved_mode_idx = eep;
            eep = eeprom_read_byte((const uint8_t *)(eepos + 1));
            if (eep != 0xff) {
                saved_ramp_level = eep;
            }
            break;
        }
    }
    #endif
}
#endif  // ifdef CONFIG_MODE

#endif
