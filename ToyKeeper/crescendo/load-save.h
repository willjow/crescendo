#ifndef LOAD_SAVE_H
#define LOAD_SAVE_H

enum eeprom_options {
#ifdef MEMTOGGLE
    EEP_MEMORY,
#endif
#ifdef THERM_CALIBRATION_MODE
    EEP_THERM_CEIL,
#endif
    EEP_OPTIONS_END
};

enum eeprom_wl_options {
    // the first wear-leveled option must be one that is never 0xff, because we
    // load from eeprom by searching for the first byte that isn't 0xff
#ifdef RAMP_MEMORY
    EEP_WL_RAMP_LEVEL,
#endif
#ifdef MODE_MEMORY
    EEP_WL_MODE_ID,
#endif
    EEP_WL_OPTIONS_END
};

#define EEP_WL_SIZE (EEPSIZE - EEP_OPTIONS_END)
#define EEP_WL_START EEP_OPTIONS_END
#define EEP_WL_END (EEP_WL_START + EEP_WL_SIZE - EEP_WL_OPTIONS_END)

#ifdef MEMORY_WL
uint8_t eepos = EEP_WL_START;
#ifdef RAMP_MEMORY
uint8_t saved_ramp_level = 1;
#endif
#ifdef MODE_MEMORY
uint8_t saved_mode_id = 0;
#endif

void save_state_wl();
#endif

#ifdef CONFIG_MODE
void save_state();
#else
#define save_state save_state_wl
#endif

#if defined(MEMORY_WL) || defined(CONFIG_MODE)
void restore_state();
#endif

#endif

