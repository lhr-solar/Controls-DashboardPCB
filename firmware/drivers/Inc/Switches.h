#pragma once

#include "pinDefs.h"

#define INTERRUPT_0_TO_4_BOUND	4
#define INTERRUPT_5_TO_9_BOUND	9


/**
 * @brief  Initializes all switch GPIO pins as inputs.
 * 
 */
void switch_GPIO_init();

/**
 * @brief  Configures a GPIO pin as an external interrupt and enables it in the NVIC
 *         Triggers on both rising and falling edges.
 * @param  port      GPIO port of the switch (e.g. GPIOA, GPIOB).
 * @param  pin       GPIO pin number (e.g. GPIO_PIN_5).
 * @param  priority  NVIC priority to assign to the interrupt.
 */
void switch_EXTI_Init(GPIO_TypeDef *port, uint16_t pin, uint32_t priority);

/**
 * @brief 	Polls all switch inputs and updates the switch_state[] array passed into paylod
 * @param payload	Pointer to uint8_t array that stores switch states to be written to/by CAN rx and tx
 * @attention 		Each entry is 1 if the switch is ON (active low), 0 if OFF.
 */
void switch_poll_all_inputs(uint8_t* payload);

/**
 * @brief  Reads the current state of a single switch.
 * @param  port  GPIO port of the switch (e.g. GPIOA, GPIOB).
 * @param  pin   GPIO pin number (e.g. GPIO_PIN_5).
 * 
 * @return ON if the pin is low (active), OFF if the pin is high.
 */
switch_state_t switch_get_state(GPIO_TypeDef *port, uint16_t pin);