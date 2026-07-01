#pragma once

#include "stm32xx_hal.h"

#define FDCAN_NVIC_PRIO		configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY

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

/**
 * @brief  Same as gpioPin_Init but with explicit pull configuration.
 * @param  pull  GPIO_NOPULL, GPIO_PULLUP, or GPIO_PULLDOWN.
 *
 * @attention Switch inputs must NEVER be GPIO_NOPULL — an unplugged or
 *            idle harness line floats and reads as a phantom press.
 */
void gpioPin_InitPull(GPIO_TypeDef *port, uint16_t pin, uint32_t mode, uint32_t pull);

