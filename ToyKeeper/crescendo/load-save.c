#ifndef LOAD_SAVE_C
#define LOAD_SAVE_C

#include "load-save.h"

#if defined(MEMORY) || defined(CONFIG_MODE)
#if (ATTINY == 85) || (ATTINY == 45)
#define EEP_WEAR_LVL_LEN 128
#elif (ATTINY == 25)
#define EEP_WEAR_LVL_LEN 64
#elif (ATTINY == 13)
#define EEP_WEAR_LVL_LEN 32
#endif
#endif

#ifdef MEMORY
void save_mode() {  // save the current mode index (with wear leveling)
    #ifdef MEMTOGGLE
    // only save when memory is enabled
    if (memory)
    #endif
    {
        eeprom_write_byte((uint8_t *)(eepos), 0xff);     // erase old state
        eeprom_write_byte((uint8_t *)(++eepos), 0xff);     // erase old state

        eepos = (eepos+1) & (EEP_WEAR_LVL_LEN-1);  // wear leveling, use next cell
        // save current mode
        eeprom_write_byte((uint8_t *)(eepos), mode_idx);
        // save current brightness
        eeprom_write_byte((uint8_t *)(eepos+1), ramp_level);
    }
}
#endif

#ifdef CONFIG_MODE
#define OPT_memory (EEP_WEAR_LVL_LEN+1)
#define OPT_therm_ceil (EEP_WEAR_LVL_LEN+2)
void save_state() {
    #ifdef MEMORY
    save_mode();
    #endif
    #ifdef MEMTOGGLE
    eeprom_write_byte((uint8_t *)OPT_memory, memory);
    #endif
    #ifdef THERM_CALIBRATION_MODE
    eeprom_write_byte((uint8_t *)OPT_therm_ceil, therm_ceil);
    #endif
}
#endif

#if defined(MEMORY) || defined(CONFIG_MODE)
void restore_state() {
    uint8_t eep;
    #ifdef MEMTOGGLE
    // memory is either 1 or 0
    // (if it's unconfigured, 0xFF, assume it's off)
    eep = eeprom_read_byte((uint8_t *)OPT_memory);
    if (eep < 2) { memory = eep; }
    else { memory = 0; }
    #endif

    #ifdef THERM_CALIBRATION_MODE
    // load therm_ceil
    eep = eeprom_read_byte((uint8_t *)OPT_therm_ceil);
    if ((eep > 0) && (eep < MAX_THERM_CEIL)) {
        therm_ceil = eep;
    }
    #endif

    #ifdef MEMORY
    // find the mode index and last brightness level
    for(eepos=0; eepos<EEP_WEAR_LVL_LEN; eepos+=2) {
        eep = eeprom_read_byte((const uint8_t *)eepos);
        if (eep != 0xff) {
            saved_mode_idx = eep;
            eep = eeprom_read_byte((const uint8_t *)(eepos+1));
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
