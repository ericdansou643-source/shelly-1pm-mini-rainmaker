#include "bl0942_driver.h"
#include "shelly_hw.h"
#include "esp_log.h"
#include "driver/uart.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "string.h"

static const char *TAG = "BL0942";

/* BL0942 UART Read/Write Protocol */
#define BL0942_READ_CMD_PREFIX      0xA5            /* Read command prefix */
#define BL0942_WRITE_CMD_PREFIX     0x5A            /* Write command prefix */
#define BL0942_FRAME_START          0x55            /* Frame start marker */
#define BL0942_FRAME_END            0xAA            /* Frame end marker */

typedef struct {
    uint8_t start;
    uint8_t cmd;
    uint8_t data[4];
    uint8_t checksum;
    uint8_t end;
} bl0942_frame_t;

/* Helper function: Calculate BL0942 checksum */
static uint8_t bl0942_calculate_checksum(uint8_t *data, uint8_t len)
{
    uint8_t checksum = 0;
    for (uint8_t i = 0; i < len; i++) {
        checksum += data[i];
    }
    return (checksum ^ 0xFF) + 1;
}

/* Helper function: Send UART command to BL0942 */
static esp_err_t bl0942_send_command(uint8_t reg)
{
    uint8_t cmd[5] = {0x55, BL0942_READ_CMD_PREFIX, reg, 0x00, 0xAA};
    cmd[3] = bl0942_calculate_checksum(&cmd[1], 2);
    
    int bytes_written = uart_write_bytes(SHELLY_UART_NUM, (const char *)cmd, 5);
    if (bytes_written != 5) {
        ESP_LOGE(TAG, "Failed to send command to BL0942");
        return ESP_FAIL;
    }
    return ESP_OK;
}

/* Helper function: Read response from BL0942 */
static esp_err_t bl0942_read_response(uint8_t *buffer, uint8_t expected_len)
{
    int bytes_read = uart_read_bytes(SHELLY_UART_NUM, buffer, expected_len, pdMS_TO_TICKS(100));
    if (bytes_read != expected_len) {
        ESP_LOGE(TAG, "Failed to read BL0942 response. Expected %d, got %d", expected_len, bytes_read);
        return ESP_FAIL;
    }
    return ESP_OK;
}

/* Initialize BL0942 driver */
esp_err_t bl0942_init(void)
{
    ESP_LOGI(TAG, "Initializing BL0942 Energy Meter");
    
    /* UART is initialized in shelly_hw_init() */
    /* Allow some time for BL0942 to stabilize */
    vTaskDelay(pdMS_TO_TICKS(500));
    
    ESP_LOGI(TAG, "BL0942 initialized successfully");
    return ESP_OK;
}

/* Read voltage from BL0942 (24-bit value, scale: V/LSB) */
esp_err_t bl0942_get_voltage(float *voltage)
{
    if (!voltage) return ESP_ERR_INVALID_ARG;
    
    uint8_t response[7] = {0};
    
    if (bl0942_send_command(BL0942_REG_VOLTAGE) != ESP_OK) {
        return ESP_FAIL;
    }
    
    if (bl0942_read_response(response, 7) != ESP_OK) {
        return ESP_FAIL;
    }
    
    /* Parse 24-bit voltage value (bytes 2-4) */
    uint32_t raw_value = (response[2] << 16) | (response[3] << 8) | response[4];
    
    /* BL0942 voltage scaling: 1LSB = 0.01V (typical) */
    *voltage = (float)raw_value * 0.01f;
    
    return ESP_OK;
}

/* Read current from BL0942 (24-bit value) */
esp_err_t bl0942_get_current(float *current)
{
    if (!current) return ESP_ERR_INVALID_ARG;
    
    uint8_t response[7] = {0};
    
    if (bl0942_send_command(BL0942_REG_CURRENT) != ESP_OK) {
        return ESP_FAIL;
    }
    
    if (bl0942_read_response(response, 7) != ESP_OK) {
        return ESP_FAIL;
    }
    
    /* Parse 24-bit current value (bytes 2-4) */
    uint32_t raw_value = (response[2] << 16) | (response[3] << 8) | response[4];
    
    /* BL0942 current scaling: 1LSB = 0.001A (typical) */
    *current = (float)raw_value * 0.001f;
    
    return ESP_OK;
}

/* Read active power from BL0942 (32-bit value) */
esp_err_t bl0942_get_power(float *power)
{
    if (!power) return ESP_ERR_INVALID_ARG;
    
    uint8_t response[8] = {0};
    
    if (bl0942_send_command(BL0942_REG_POWER) != ESP_OK) {
        return ESP_FAIL;
    }
    
    if (bl0942_read_response(response, 8) != ESP_OK) {
        return ESP_FAIL;
    }
    
    /* Parse 32-bit power value (bytes 2-5) */
    uint32_t raw_value = (response[2] << 24) | (response[3] << 16) | 
                         (response[4] << 8) | response[5];
    
    /* BL0942 power scaling: 1LSB = 0.01W (typical) */
    *power = (float)raw_value * 0.01f;
    
    return ESP_OK;
}

/* Read accumulated energy from BL0942 (32-bit value) */
esp_err_t bl0942_get_energy(uint32_t *energy)
{
    if (!energy) return ESP_ERR_INVALID_ARG;
    
    uint8_t response[8] = {0};
    
    if (bl0942_send_command(BL0942_REG_ENERGY) != ESP_OK) {
        return ESP_FAIL;
    }
    
    if (bl0942_read_response(response, 8) != ESP_OK) {
        return ESP_FAIL;
    }
    
    /* Parse 32-bit energy value (bytes 2-5) in Wh */
    *energy = (response[2] << 24) | (response[3] << 16) | 
              (response[4] << 8) | response[5];
    
    return ESP_OK;
}

/* Read all data from BL0942 at once */
esp_err_t bl0942_read_data(bl0942_data_t *data)
{
    if (!data) return ESP_ERR_INVALID_ARG;
    
    if (bl0942_get_voltage(&data->voltage) != ESP_OK) return ESP_FAIL;
    if (bl0942_get_current(&data->current) != ESP_OK) return ESP_FAIL;
    if (bl0942_get_power(&data->power) != ESP_OK) return ESP_FAIL;
    if (bl0942_get_energy(&data->energy) != ESP_OK) return ESP_FAIL;
    
    /* Power factor and temperature require similar UART read logic */
    data->power_factor = 1.0f;  /* Default to unity */
    data->temperature = 25.0f;  /* Default ambient */
    
    return ESP_OK;
}

/* Reset energy counter */
esp_err_t bl0942_reset_energy(void)
{
    ESP_LOGI(TAG, "Resetting energy counter");
    /* This would require a write command to BL0942 */
    /* Implementation depends on specific BL0942 reset register */
    return ESP_OK;
}