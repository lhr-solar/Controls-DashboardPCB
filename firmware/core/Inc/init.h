#pragma once

#include "stm32xx_hal.h"


/**
 * 
 * @brief System Clock Configuration - Cube MX
 * 
 */
void SystemClock_Config(void);

/**
 * @brief  Initializes a single GPIO pin with the specified mode.
 * @param  port  GPIO port (e.g. GPIOA, GPIOB).
 * @param  pin   GPIO pin number (e.g. GPIO_PIN_10, GPIO_PIN_3).
 * @param  mode  GPIO mode (e.g. GPIO_MODE_OUTPUT_PP, GPIO_MODE_INPUT).
 *               
 * @attention if mode is not output, pin is driven low on init.
 */
void gpioPin_Init(GPIO_TypeDef *port, uint16_t pin, uint32_t mode);
