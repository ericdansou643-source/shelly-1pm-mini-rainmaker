#ifndef RELAY_CONTROL_H
#define RELAY_CONTROL_H

#include "esp_err.h"
#include <stdbool.h>

/* ============================================================================
   SHELLY 1PM MINI GEN3 - RELAY CONTROL
   ============================================================================ */

typedef struct {
    bool power;                     /* Relay state (on/off) */
    bool button_enabled;            /* Physical button enabled */
    bool overload_protected;        /* Overload protection active */
} relay_state_t;

/* Function Declarations */
esp_err_t relay_init(void);
esp_err_t relay_set_power(bool power);
bool relay_get_power(void);
esp_err_t relay_toggle(void);
esp_err_t relay_set_overload_protection(bool enabled);
bool relay_get_overload_status(void);

#endif /* RELAY_CONTROL_H */