#pragma once

#include "stm32xx_hal.h"

#define COMMON_TASK_DELAY_TICKS		pdMS_TO_TICKS(250)
#define CL_MAX_PRIO					tskIDLE_PRIORITY + 5


/**
 * 
 * @brief Controls Leade Status Message
 * 
 */
typedef enum CL_status_t {
    CL_EVENT_GROUP_INIT_FAIL,
	CL_OK
} CL_status_t;


/**
 * @brief  Switch states are active low — ON maps to GPIO_PIN_RESET,
 *         OFF maps to GPIO_PIN_SET.
 */
typedef enum {
    SWITCH_ON  = GPIO_PIN_RESET,
    SWITCH_OFF = GPIO_PIN_SET
} switch_state_t;



/**
 * @brief  Adresses switches to specific index on
 * 		   data payload to be sent on CAN
 * 
 */
typedef enum {
    SW_IGN_ARR       = 0,
    SW_IGN_MTR       = 1,
    SW_CRUISE_ENABLE = 2,
    SW_CRUISE_SET    = 3,
    SW_FWD           = 4,
    SW_NEUTRAL_GEAR  = 5,
    SW_REV           = 6,
    SW_HAZARD        = 7,
    SW_LEFT_BLINKER  = 8,
    SW_RIGHT_BLINKER = 9,
    SW_HORN          = 10,
    SW_PTT           = 11,
    SW_REGEN_ENABLE  = 12,
    SW_REGEN_ACTIVE  = 13,
	SW_TBD			 = 14,
    SW_COUNT         = 15   // total number of switches, use for array size
} state_bit_t;

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