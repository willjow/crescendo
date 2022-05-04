#ifndef HELPER_MACROS_H
#define HELPER_MACROS_H

/* ==== Helper Macros ====================================================== */
#if defined(VOLTAGE_PROTECTION) || defined(BATTCHECK)
#define VOLTAGE_MON
#endif

#if defined(OFFTIM2) || defined(OFFTIM3)
#define OFFTIME
#endif

#if defined(RAMP_MEMORY) || defined(MODE_MEMORY)
#define MEMORY_WL
#endif

#if defined(MEMORY_WL) || defined(MANUAL_LEVEL)
#define MEMORY
#endif

#if defined(THERM_CALIBRATION_MODE)
#define USE_BLINK_NUM
#endif

#if defined(MEMTOGGLE) || defined(THERM_CALIBRATION_MODE)
#define CONFIG_MODE
#endif

#endif
