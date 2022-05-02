#ifndef VOLTAGE_MONITOR_H
#define VOLTAGE_MONITOR_H

#ifdef BATTCHECK
void battcheck_mode();
#endif

#ifdef VOLTAGE_PROTECTION
void monitor_voltage(uint8_t mode, uint8_t *lowbatt_cnt);
#endif

#endif

