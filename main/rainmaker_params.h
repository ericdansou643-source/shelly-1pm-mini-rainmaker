#ifndef RAINMAKER_PARAMS_H
#define RAINMAKER_PARAMS_H

#include "esp_err.h"
#include "esp_rmaker_core.h"

/* ============================================================================
   SHELLY 1PM MINI GEN3 - ESP RAINMAKER INTEGRATION
   ============================================================================ */

/* Device parameters for RainMaker */
#define SHELLY_DEVICE_NAME          "Shelly 1PM Mini"
#define SHELLY_DEVICE_MODEL         "S3PM-001PCEU16"

/* Function Declarations */
esp_err_t rainmaker_params_init(esp_rmaker_device_t *device);
esp_err_t rainmaker_update_power(esp_rmaker_device_t *device, bool power);
esp_err_t rainmaker_update_voltage(esp_rmaker_device_t *device, float voltage);
esp_err_t rainmaker_update_current(esp_rmaker_device_t *device, float current);
esp_err_t rainmaker_update_power_consumption(esp_rmaker_device_t *device, float power);
esp_err_t rainmaker_update_energy(esp_rmaker_device_t *device, uint32_t energy);

#endif /* RAINMAKER_PARAMS_H */