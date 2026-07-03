#include "rainmaker_params.h"
#include "relay_control.h"
#include "power_monitor.h"
#include "esp_log.h"
#include "esp_rmaker_standard_params.h"
#include "esp_rmaker_standard_devices.h"

static const char *TAG = "RAINMAKER";

/* Write callback for Power parameter */
static esp_err_t power_write_cb(const esp_rmaker_param_val_t val, void *priv, esp_rmaker_write_ctx_t *ctx)
{
    bool power = val.val.b;
    ESP_LOGI(TAG, "Received power command: %s", power ? "ON" : "OFF");
    
    if (relay_set_power(power) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to set relay power");
        return ESP_FAIL;
    }
    
    return ESP_OK;
}

/* Read callback for Power parameter */
static esp_err_t power_read_cb(const void *priv, esp_rmaker_param_val_t *val, esp_rmaker_read_ctx_t *ctx)
{
    val->val.b = relay_get_power();
    return ESP_OK;
}

/* Initialize RainMaker parameters */
esp_err_t rainmaker_params_init(esp_rmaker_device_t *device)
{
    ESP_LOGI(TAG, "Initializing RainMaker parameters");
    
    if (!device) {
        ESP_LOGE(TAG, "Invalid device pointer");
        return ESP_ERR_INVALID_ARG;
    }
    
    /* Power parameter (read/write) */
    esp_rmaker_param_t *power_param = esp_rmaker_param_create(
        "Power",
        NULL,
        esp_rmaker_bool(false),
        PROP_FLAG_READ | PROP_FLAG_WRITE | PROP_FLAG_PERSIST
    );
    
    if (!power_param) {
        ESP_LOGE(TAG, "Failed to create Power parameter");
        return ESP_FAIL;
    }
    
    esp_rmaker_param_add_cb(power_param, power_write_cb, power_read_cb, NULL);
    esp_rmaker_device_add_param(device, power_param);
    
    /* Voltage parameter (read-only) */
    esp_rmaker_param_t *voltage_param = esp_rmaker_param_create(
        "Voltage",
        "voltage",
        esp_rmaker_float(0.0),
        PROP_FLAG_READ
    );
    esp_rmaker_param_add_ui_type(voltage_param, ESP_RMAKER_UI_SLIDER);
    esp_rmaker_device_add_param(device, voltage_param);
    
    /* Current parameter (read-only) */
    esp_rmaker_param_t *current_param = esp_rmaker_param_create(
        "Current",
        "current",
        esp_rmaker_float(0.0),
        PROP_FLAG_READ
    );
    esp_rmaker_param_add_ui_type(current_param, ESP_RMAKER_UI_SLIDER);
    esp_rmaker_device_add_param(device, current_param);
    
    /* Power consumption parameter (read-only) */
    esp_rmaker_param_t *power_consumption_param = esp_rmaker_param_create(
        "Power Consumption",
        "power_consumption",
        esp_rmaker_float(0.0),
        PROP_FLAG_READ
    );
    esp_rmaker_param_add_ui_type(power_consumption_param, ESP_RMAKER_UI_SLIDER);
    esp_rmaker_device_add_param(device, power_consumption_param);
    
    /* Energy parameter (read-only) */
    esp_rmaker_param_t *energy_param = esp_rmaker_param_create(
        "Energy",
        "energy",
        esp_rmaker_float(0.0),
        PROP_FLAG_READ
    );
    esp_rmaker_param_add_ui_type(energy_param, ESP_RMAKER_UI_SLIDER);
    esp_rmaker_device_add_param(device, energy_param);
    
    ESP_LOGI(TAG, "RainMaker parameters initialized successfully");
    return ESP_OK;
}

/* Update Power parameter in RainMaker */
esp_err_t rainmaker_update_power(esp_rmaker_device_t *device, bool power)
{
    return esp_rmaker_param_update_and_report(
        esp_rmaker_device_get_param_by_name(device, "Power"),
        esp_rmaker_bool(power)
    );
}

/* Update Voltage parameter in RainMaker */
esp_err_t rainmaker_update_voltage(esp_rmaker_device_t *device, float voltage)
{
    return esp_rmaker_param_update_and_report(
        esp_rmaker_device_get_param_by_name(device, "Voltage"),
        esp_rmaker_float(voltage)
    );
}

/* Update Current parameter in RainMaker */
esp_err_t rainmaker_update_current(esp_rmaker_device_t *device, float current)
{
    return esp_rmaker_param_update_and_report(
        esp_rmaker_device_get_param_by_name(device, "Current"),
        esp_rmaker_float(current)
    );
}

/* Update Power Consumption parameter in RainMaker */
esp_err_t rainmaker_update_power_consumption(esp_rmaker_device_t *device, float power)
{
    return esp_rmaker_param_update_and_report(
        esp_rmaker_device_get_param_by_name(device, "Power Consumption"),
        esp_rmaker_float(power)
    );
}

/* Update Energy parameter in RainMaker */
esp_err_t rainmaker_update_energy(esp_rmaker_device_t *device, uint32_t energy)
{
    return esp_rmaker_param_update_and_report(
        esp_rmaker_device_get_param_by_name(device, "Energy"),
        esp_rmaker_float((float)energy)
    );
}