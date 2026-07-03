#ifndef POWER_MONITOR_H
#define POWER_MONITOR_H

#include "esp_err.h"
#include <stdint.h>

/* ============================================================================
   SHELLY 1PM MINI GEN3 - POWER MONITORING
   ============================================================================ */

typedef struct {
    float voltage;                  /* Current voltage (V) */
    float current;                  /* Current current (A) */
    float power;                    /* Current power (W) */
    uint32_t energy;                /* Accumulated energy (Wh) */
    float power_factor;             /* Power factor (0.0-1.0) */
} power_data_t;

/* Function Declarations */
esp_err_t power_monitor_init(void);
esp_err_t power_monitor_update(void);
power_data_t power_monitor_get_data(void);
esp_err_t power_monitor_reset_energy(void);

#endif /* POWER_MONITOR_H */