#ifndef BL0942_DRIVER_H
#define BL0942_DRIVER_H

#include <stdint.h>
#include <stdbool.h>
#include "esp_err.h"

/* ============================================================================
   BL0942 Energy Metering IC - UART Driver
   ============================================================================ */

/* BL0942 Register Map (Read Commands) */
#define BL0942_REG_VOLTAGE          0x06            /* Voltage RMS (4 bytes) */
#define BL0942_REG_CURRENT          0x07            /* Current RMS (3 bytes) */
#define BL0942_REG_POWER            0x08            /* Active Power (4 bytes) */
#define BL0942_REG_ENERGY           0x09            /* Energy Accumulation (4 bytes) */
#define BL0942_REG_POWER_FACTOR     0x0A            /* Power Factor (2 bytes) */
#define BL0942_REG_TEMP             0x0B            /* Temperature (2 bytes) */

/* BL0942 Data Structure */
typedef struct {
    float voltage;                  /* Voltage in V */
    float current;                  /* Current in A */
    float power;                    /* Active Power in W */
    uint32_t energy;                /* Energy in Wh */
    float power_factor;             /* Power Factor (0.0 - 1.0) */
    float temperature;              /* Temperature in °C */
} bl0942_data_t;

/* Function Declarations */
esp_err_t bl0942_init(void);
esp_err_t bl0942_read_data(bl0942_data_t *data);
esp_err_t bl0942_get_voltage(float *voltage);
esp_err_t bl0942_get_current(float *current);
esp_err_t bl0942_get_power(float *power);
esp_err_t bl0942_get_energy(uint32_t *energy);
esp_err_t bl0942_reset_energy(void);

#endif /* BL0942_DRIVER_H */