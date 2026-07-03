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
	ON,
	OFF
} high_noon_state_t;

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
    SW_IGN_ARR       = 0,   // bit 0  - Ignition_Array
    SW_IGN_MTR       = 1,   // bit 1  - Ignition_Motor
    SW_IGN_OFF       = 2,   // bit 2  - Ignition_Off
    SW_CRUISE_ENABLE = 3,   // bit 3  - Cruise_Enable
    SW_CRUISE_SET    = 4,   // bit 4  - Cruise_Set
    SW_FWD           = 5,   // bit 5  - Gear_Forward
    SW_NEUTRAL_GEAR  = 6,   // bit 6  - Gear_Neutral
    SW_REV           = 7,   // bit 7  - Gear_Reverse
    SW_HAZARD        = 8,   // bit 8  - Hazard_Pressed
    SW_HORN          = 9,   // bit 9  - Horn_Pressed
    SW_LEFT_BLINKER  = 10,  // bit 10 - Blinker_Left
    SW_RIGHT_BLINKER = 11,  // bit 11 - Blinker_Right
    SW_PTT           = 12,  // bit 12 - PushToTalk_Pressed
    SW_REGEN_ACTIVE  = 13,  // bit 13 - Regen_Activate
    SW_REGEN_ENABLE  = 14,  // bit 14 - Regen_Enable
    SW_COUNT
} switch_bit_t;

extern const char *const switch_names[SW_COUNT];

typedef enum {
	VCU_REGEN_STATUS = 0,  // bit 0 - VCU Regen Status read from CarCAN
	BPS_FAULT = 1,		   // bit 1 - BPS faults read from CarCAN
	VCU_BRAKE_STATUS = 2,	   // bit 2 - Brake status read from CarCAN
	STATUS_COUNT
} high_noon_state_bit_t;


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

high_noon_state_t get_high_noon_state(high_noon_state_bit_t b);

/**
 * @brief Set a state in the fault bitmap
 *
 * @param bit 	which fault is being set
 * @param state SWITCH_ON - 1, SWITCH_OFF - 0 = what you want to set the bit to
 * 
 * @return switch_bitmaps bitmap
 */
uint32_t switch_bitmap_setBit(switch_bit_t bit, switch_state_t state);

uint32_t set_high_noon_state(high_noon_state_bit_t bit, high_noon_state_t state);

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
