#ifndef SHELLY_HW_H
#define SHELLY_HW_H

#include "driver/gpio.h"
#include "driver/uart.h"

/* ============================================================================
   SHELLY 1PM MINI GEN3 - GPIO CONFIGURATION
   ESP32-C3 Based Device
   ============================================================================ */

/* GPIO Pin Definitions */
#define SHELLY_GPIO_LED             GPIO_NUM_0      /* Status LED */
#define SHELLY_GPIO_BUTTON          GPIO_NUM_1      /* Physical Button */
#define SHELLY_GPIO_NTC_TEMP        GPIO_NUM_3      /* NTC Temperature Sensor (ADC) */
#define SHELLY_GPIO_RELAY           GPIO_NUM_4      /* Relay Output (Main Control) */
#define SHELLY_GPIO_BL0942_TX       GPIO_NUM_6      /* BL0942 UART TX */
#define SHELLY_GPIO_BL0942_RX       GPIO_NUM_7      /* BL0942 UART RX */
#define SHELLY_GPIO_SWITCH_INPUT    GPIO_NUM_10     /* Switch/Pushbutton Input */

/* UART Configuration for BL0942 Energy Meter */
#define SHELLY_UART_NUM             UART_NUM_1
#define SHELLY_UART_BAUD            4800            /* BL0942 default baud rate */
#define SHELLY_UART_RX_BUF_SIZE     256
#define SHELLY_UART_TX_BUF_SIZE     128

/* LED Control */
#define SHELLY_LED_ON()             gpio_set_level(SHELLY_GPIO_LED, 1)
#define SHELLY_LED_OFF()            gpio_set_level(SHELLY_GPIO_LED, 0)
#define SHELLY_LED_TOGGLE()         gpio_set_level(SHELLY_GPIO_LED, !gpio_get_level(SHELLY_GPIO_LED))

/* Relay Control */
#define SHELLY_RELAY_ON()           gpio_set_level(SHELLY_GPIO_RELAY, 1)
#define SHELLY_RELAY_OFF()          gpio_set_level(SHELLY_GPIO_RELAY, 0)
#define SHELLY_RELAY_GET_STATE()    gpio_get_level(SHELLY_GPIO_RELAY)

/* Function Declarations */
void shelly_hw_init(void);
void shelly_gpio_init(void);
void shelly_uart_init(void);

#endif /* SHELLY_HW_H */