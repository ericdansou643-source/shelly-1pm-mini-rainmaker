#include "relay_control.h"
#include "shelly_hw.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

static const char *TAG = "RELAY";

static relay_state_t relay_state = {
    .power = false,
    .button_enabled = true,
    .overload_protected = false
};

/* Initialize relay control */
esp_err_t relay_init(void)
{
    ESP_LOGI(TAG, "Initializing Relay Control");
    
    /* Configure relay GPIO as output */
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << SHELLY_GPIO_RELAY),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = 0,
        .pull_down_en = 0,
        .intr_type = GPIO_INTR_DISABLE
    };
    
    if (gpio_config(&io_conf) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to configure relay GPIO");
        return ESP_FAIL;
    }
    
    /* Initialize relay to OFF state */
    SHELLY_RELAY_OFF();
    relay_state.power = false;
    
    ESP_LOGI(TAG, "Relay initialized successfully");
    return ESP_OK;
}

/* Set relay power state */
esp_err_t relay_set_power(bool power)
{
    ESP_LOGI(TAG, "Setting relay power: %s", power ? "ON" : "OFF");
    
    if (power) {
        SHELLY_RELAY_ON();
    } else {
        SHELLY_RELAY_OFF();
    }
    
    relay_state.power = power;
    return ESP_OK;
}

/* Get current relay state */
bool relay_get_power(void)
{
    return relay_state.power;
}

/* Toggle relay state */
esp_err_t relay_toggle(void)
{
    return relay_set_power(!relay_state.power);
}

/* Set overload protection */
esp_err_t relay_set_overload_protection(bool enabled)
{
    relay_state.overload_protected = enabled;
    ESP_LOGI(TAG, "Overload protection: %s", enabled ? "ENABLED" : "DISABLED");
    return ESP_OK;
}

/* Get overload status */
bool relay_get_overload_status(void)
{
    return relay_state.overload_protected;
}