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
#ifdef MEMORY
    EEP_WL_MODE_IDX,
    EEP_WL_RAMP_LEVEL,
#endif
    EEP_WL_OPTIONS_END
};

#define EEP_WL_SIZE (EEPSIZE - EEP_OPTIONS_END)
#define EEP_WL_START EEP_OPTIONS_END
#define EEP_WL_END (EEP_WL_START + EEP_WL_SIZE - EEP_WL_OPTIONS_END)

#ifdef MEMORY
void save_mode();
#endif

#ifdef CONFIG_MODE
void save_state();
#else
#define save_state save_mode
#endif

#if defined(MEMORY) || defined(CONFIG_MODE)
void restore_state();
#endif

#endif

