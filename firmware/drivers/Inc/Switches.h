#pragma once

#include "pinDefs.h"

#define INTERRUPT_0_TO_4_BOUND	4
#define INTERRUPT_5_TO_9_BOUND	9

//convert state_bit_t -> bit mask
#define GET_MASK(bit)   ((uint32_t)(1U << (bit)))

/**
 * @brief  Initializes all switch GPIO pins as inputs.
 * 
 */
void switch_GPIO_init();

/**
 * @brief  Initializes a GPIO pin as an external interrupt source,
 *         triggering on both rising and falling edges.
 * @param  port  GPIO port (e.g. GPIOA, GPIOB).
 * @param  pin   GPIO pin number (e.g. GPIO_PIN_10, GPIO_PIN_3).
 */
void gpioEXTI_Init(GPIO_TypeDef *port, uint16_t pin);

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
 * 
 * @attention 		Each entry is 1 if the switch is ON (active low), 0 if OFF.
 * 
 * @return switch_bitmaps bitmap
 */
uint32_t switch_read_all_inputs();

/**
 * @brief  Reads the current state of a single switch.
 * @param  port  GPIO port of the switch (e.g. GPIOA, GPIOB).
 * @param  pin   GPIO pin number (e.g. GPIO_PIN_5).
 * 
 * @return ON if the pin is low (active), OFF if the pin is high.
 */
switch_state_t switch_get_state(GPIO_TypeDef *port, uint16_t pin);


/**
 * @brief Set a state in the fault bitmap
 *
 * @param bit 	which fault is being set
 * @param state SWITCH_ON - 1, SWITCH_OFF - 0 = what you want to set the bit to
 * 
 * @return switch_bitmaps bitmap
 */
uint32_t switch_bitmap_setBit(state_bit_t bit, switch_state_t state);

/**
 * @brief Set all state bits
 *
 * @param bits new bitmap with all switch states
 * 
 * @return switch_bitmaps bitmap
 */
uint32_t switch_bitmap_setAll(uint32_t bits);

/**
 * @brief returns switch_bitmaps bitmap
 *
 * @return switch_bitmaps bitmap
 */
uint32_t switch_bitmap_read();