#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "nvs_flash.h"
#include "esp_rmaker_core.h"
#include "esp_rmaker_standard_types.h"
#include "esp_rmaker_standard_params.h"
#include "esp_rmaker_standard_devices.h"
#include "app_wifi.h"
#include "app_ota.h"

#include "shelly_hw.h"
#include "relay_control.h"
#include "power_monitor.h"
#include "rainmaker_params.h"

static const char *TAG = "SHELLY_RAINMAKER";

/* Global device handle */
static esp_rmaker_device_t *shelly_device = NULL;

/* RainMaker event handler */
static void rainmaker_event_handler(void* arg, esp_event_base_t event_base,
                                    int32_t event_id, void* event_data)
{
    if (event_base == RMAKER_EVENT) {
        switch (event_id) {
            case RMAKER_EVENT_INIT_DONE:
                ESP_LOGI(TAG, "RainMaker core initialized");
                break;
            case RMAKER_EVENT_CLAIM_STARTED:
                ESP_LOGI(TAG, "RainMaker claim started");
                break;
            case RMAKER_EVENT_CLAIM_SUCCESSFUL:
                ESP_LOGI(TAG, "RainMaker claim successful");
                break;
            case RMAKER_EVENT_CLAIM_FAILED:
                ESP_LOGE(TAG, "RainMaker claim failed");
                break;
            default:
                break;
        }
    }
}

/* WiFi event handler */
static void wifi_event_handler(void* arg, esp_event_base_t event_base,
                               int32_t event_id, void* event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_CONNECTED) {
        ESP_LOGI(TAG, "WiFi connected");
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ESP_LOGI(TAG, "Got IP address");
    }
}

/* Data update task - periodically update RainMaker with sensor data */
static void data_update_task(void *arg)
{
    power_data_t power_data;
    
    ESP_LOGI(TAG, "Data update task started");
    
    while (1) {
        if (shelly_device) {
            power_data = power_monitor_get_data();
            
            /* Update all parameters */
            rainmaker_update_voltage(shelly_device, power_data.voltage);
            rainmaker_update_current(shelly_device, power_data.current);
            rainmaker_update_power_consumption(shelly_device, power_data.power);
            rainmaker_update_energy(shelly_device, power_data.energy);
            
            ESP_LOGD(TAG, "Updated RainMaker: U=%.2fV I=%.3fA P=%.1fW E=%ldWh",
                    power_data.voltage, power_data.current, 
                    power_data.power, power_data.energy);
        }
        
        /* Update every 5 seconds */
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}

void app_main(void)
{
    esp_err_t err = ESP_OK;
    
    /* Initialize NVS (required for RainMaker) */
    err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        nvs_flash_erase();
        err = nvs_flash_init();
    }
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "NVS initialization failed");
        return;
    }
    
    /* Initialize network interface */
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    
    /* Register event handlers */
    ESP_ERROR_CHECK(esp_event_handler_register(RMAKER_EVENT, ESP_EVENT_ANY_ID, 
                                               &rainmaker_event_handler, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, 
                                               &wifi_event_handler, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, ESP_EVENT_ANY_ID, 
                                               &wifi_event_handler, NULL));
    
    /* Initialize hardware */
    ESP_LOGI(TAG, "Initializing Shelly 1PM Mini Gen3 Hardware");
    shelly_hw_init();
    relay_init();
    power_monitor_init();
    
    /* Initialize WiFi */
    app_wifi_init();
    
    /* Initialize RainMaker */
    esp_rmaker_config_t rainmaker_config = RMAKER_DEFAULT_CONFIG();
    rainmaker_config.enable_time_sync = true;
    
    err = esp_rmaker_init(&rainmaker_config);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "RainMaker initialization failed");
        return;
    }
    
    /* Create RainMaker device */
    shelly_device = esp_rmaker_device_create(SHELLY_DEVICE_NAME, SHELLY_DEVICE_MODEL, NULL);
    if (!shelly_device) {
        ESP_LOGE(TAG, "Failed to create RainMaker device");
        return;
    }
    
    /* Initialize RainMaker parameters */
    rainmaker_params_init(shelly_device);
    
    /* Add device to node */
    esp_rmaker_node_t *node = esp_rmaker_get_node();
    esp_rmaker_node_add_device(node, shelly_device);
    
    /* Enable OTA */
    app_ota_enable();
    
    /* Start RainMaker */
    err = esp_rmaker_start();
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "RainMaker start failed");
        return;
    }
    
    ESP_LOGI(TAG, "RainMaker started successfully");
    
    /* Create data update task */
    xTaskCreate(data_update_task, "data_update", 4096, NULL, 2, NULL);
    
    /* Application is now running */
    ESP_LOGI(TAG, "Shelly 1PM Mini Gen3 with RainMaker initialized successfully!");
    
    while (1) {
        vTaskDelay(pdMS_TO_TICKS(10000));
    }
}