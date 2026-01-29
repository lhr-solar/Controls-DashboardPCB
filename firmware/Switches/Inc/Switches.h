#include "pinMode.h"

#ifndef SWITCHES_H
#define SWITCHES_H

// Port A - input
static const GPIO_Pin FWD_SW        = { GPIOA, GPIO_PIN_0 };
static const GPIO_Pin Ign_OFF       = { GPIOA, GPIO_PIN_2 };
static const GPIO_Pin Ign_MTR       = { GPIOA, GPIO_PIN_1 };

// Port B - input
static const GPIO_Pin PTT           = { GPIOB, GPIO_PIN_2 };
static const GPIO_Pin Ign_ARR       = { GPIOB, GPIO_PIN_0 };

// Port C - input
static const GPIO_Pin Neutral_Gear  = { GPIOC, GPIO_PIN_0 };
static const GPIO_Pin Regen_Enable  = { GPIOC, GPIO_PIN_2 };
static const GPIO_Pin Right_Blinker = { GPIOC, GPIO_PIN_4 };
static const GPIO_Pin Left_Blinker  = { GPIOC, GPIO_PIN_5 };
static const GPIO_Pin Cruise_Enable = { GPIOC, GPIO_PIN_6 };
static const GPIO_Pin Rev_SW        = { GPIOC, GPIO_PIN_7 };
static const GPIO_Pin Cruise_Set    = { GPIOC, GPIO_PIN_9 };
static const GPIO_Pin Regen_Active  = { GPIOC, GPIO_PIN_12 };
static const GPIO_Pin Hazard        = { GPIOC, GPIO_PIN_8 };

// Port D - input
static const GPIO_Pin Horn          = { GPIOD, GPIO_PIN_2 };

typedef enum {
    ON = 1,
    OFF = 0
} SwitchState;

SwitchState get_switch_state(GPIO_Pin sw);

#endif