#pragma once

#include "pinDefs.h"

/**
 * @brief  Initializes all LED GPIO pins as push-pull outputs.
 * 
 */
void led_GPIO_init();

/**
 * @brief  Sets an LED to the specified state (on or off).
 * @param  port   GPIO port of the LED (e.g. GPIOA, GPIOB).
 * @param  pin    GPIO pin number (e.g. GPIO_PIN_10, GPIO_PIN_3).
 * @param  state  Desired pin state (GPIO_PIN_SET or GPIO_PIN_RESET).
 */
void led_set(GPIO_TypeDef *port, uint16_t pin, GPIO_PinState state);

/**
 * @brief  Toggles an LED from its current state.
 * @param  port  GPIO port of the LED (e.g. GPIOA, GPIOB).
 * @param  pin   GPIO pin number (e.g. GPIO_PIN_10, GPIO_PIN_3).
 */
void led_toggle(GPIO_TypeDef *port, uint16_t pin);

/**
 * @brief  Flashes all LEDs on and off together once with a 250ms on/off period.
 * 
 */
void flash_them();