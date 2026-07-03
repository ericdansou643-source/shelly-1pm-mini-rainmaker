#include "power_monitor.h"
#include "bl0942_driver.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "POWER_MONITOR";

static power_data_t current_data = {0};
static TaskHandle_t monitor_task_handle = NULL;

/* Power monitoring task */
static void power_monitor_task(void *arg)
{
    bl0942_data_t bl_data = {0};
    
    ESP_LOGI(TAG, "Power monitoring task started");
    
    while (1) {
        if (bl0942_read_data(&bl_data) == ESP_OK) {
            current_data.voltage = bl_data.voltage;
            current_data.current = bl_data.current;
            current_data.power = bl_data.power;
            current_data.energy = bl_data.energy;
            current_data.power_factor = bl_data.power_factor;
            
            ESP_LOGD(TAG, "U=%.2fV I=%.3fA P=%.1fW E=%ldWh", 
                    current_data.voltage, 
                    current_data.current, 
                    current_data.power, 
                    current_data.energy);
        } else {
            ESP_LOGE(TAG, "Failed to read power data");
        }
        
        /* Update every 2 seconds */
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

/* Initialize power monitor */
esp_err_t power_monitor_init(void)
{
    ESP_LOGI(TAG, "Initializing Power Monitor");
    
    if (bl0942_init() != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize BL0942");
        return ESP_FAIL;
    }
    
    /* Create monitoring task */
    if (xTaskCreate(power_monitor_task, "power_monitor", 4096, NULL, 3, &monitor_task_handle) != pdPASS) {
        ESP_LOGE(TAG, "Failed to create power monitor task");
        return ESP_FAIL;
    }
    
    ESP_LOGI(TAG, "Power Monitor initialized successfully");
    return ESP_OK;
}

/* Update power data (can be called manually if needed) */
esp_err_t power_monitor_update(void)
{
    bl0942_data_t bl_data = {0};
    
    if (bl0942_read_data(&bl_data) != ESP_OK) {
        return ESP_FAIL;
    }
    
    current_data.voltage = bl_data.voltage;
    current_data.current = bl_data.current;
    current_data.power = bl_data.power;
    current_data.energy = bl_data.energy;
    current_data.power_factor = bl_data.power_factor;
    
    return ESP_OK;
}

/* Get current power data */
power_data_t power_monitor_get_data(void)
{
    return current_data;
}

/* Reset energy counter */
esp_err_t power_monitor_reset_energy(void)
{
    return bl0942_reset_energy();
}