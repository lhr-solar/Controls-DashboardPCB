#pragma once

#include "pinDefs.h"

#define INTERRUPT_0_TO_4_BOUND	4
#define INTERRUPT_5_TO_9_BOUND	9

#define SWITCH_MAX_PRIO				configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY

//convert switch_bit_t -> bit mask
#define GET_MASK(bit)   ((uint32_t)(1U << (bit)))

/**
 * @brief  Switch states are active low — ON maps to GPIO_PIN_RESET,
 *         OFF maps to GPIO_PIN_SET.
 */
typedef enum {
    SWITCH_ON  = GPIO_PIN_RESET,
    SWITCH_OFF = GPIO_PIN_SET
} switch_state_t;

typedef enum {
    LED_ON  = GPIO_PIN_SET,
    LED_OFF = GPIO_PIN_RESET
} led_state_t;

/**
 * @brief  Adresses switches to specific index on
 * 		   data payload to be sent on CAN
 * 
 */
typedef enum {
    SW_IGN_OFF       = 0,
    SW_IGN_ARR       = 1,
    SW_IGN_MTR       = 2,
    SW_CRUISE_ENABLE = 3,
    SW_CRUISE_SET    = 4,
    SW_FWD           = 5,
    SW_NEUTRAL_GEAR  = 6,
    SW_REV           = 7,
    SW_HAZARD        = 8,
    SW_LEFT_BLINKER  = 9,
    SW_RIGHT_BLINKER = 10,
    SW_HORN          = 11,
    SW_PTT           = 12,
    SW_REGEN_ENABLE  = 13,
    SW_REGEN_ACTIVE  = 14,
    SW_COUNT         = 15
} switch_bit_t;



/**
 * @brief  Initializes all switch GPIO pins as inputs.
 * 
 */
void switch_init();

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
 * @return SWITCH_ON = logic high, SWITCH_OFF = logic low.
 */
switch_state_t switch_get_state(switch_bit_t sw);


/**
 * @brief Set a state in the fault bitmap
 *
 * @param bit 	which fault is being set
 * @param state SWITCH_ON - 1, SWITCH_OFF - 0 = what you want to set the bit to
 * 
 * @return switch_bitmaps bitmap
 */
uint32_t switch_bitmap_setBit(switch_bit_t bit, switch_state_t state);

/**
 * @brief Set all state bits
 *
 * @param bits new bitmap with all switch states
 * 
 * @return none
 */
void switch_bitmap_setAll(uint32_t bits);

/**
 * @brief returns switch_bitmaps bitmap
 *
 * @return switch_bitmaps bitmap
 */
uint32_t switch_bitmap_read();
