#include "Switches.h"

// Port A - input
const GPIO_Pin FWD_SW        = { GPIOA, GPIO_PIN_0 };
const GPIO_Pin Ign_OFF       = { GPIOA, GPIO_PIN_2 };
const GPIO_Pin Ign_MTR       = { GPIOA, GPIO_PIN_1 };

// Port B - input

const GPIO_Pin Ign_ARR       = { GPIOB, GPIO_PIN_0 };
const GPIO_Pin PTT           = { GPIOB, GPIO_PIN_2 };

// Port C - input
const GPIO_Pin Neutral_Gear  = { GPIOC, GPIO_PIN_0 };
const GPIO_Pin Rev_SW        = { GPIOC, GPIO_PIN_7 };

const GPIO_Pin Regen_Enable  = { GPIOC, GPIO_PIN_2 };
const GPIO_Pin Right_Blinker = { GPIOC, GPIO_PIN_4 };
const GPIO_Pin Left_Blinker  = { GPIOC, GPIO_PIN_5 };
const GPIO_Pin Cruise_Enable = { GPIOC, GPIO_PIN_6 };
const GPIO_Pin Cruise_Set    = { GPIOC, GPIO_PIN_9 };
const GPIO_Pin Regen_Active  = { GPIOC, GPIO_PIN_12 };
const GPIO_Pin Hazard        = { GPIOC, GPIO_PIN_8 };

// Port D - input
const GPIO_Pin Horn          = { GPIOD, GPIO_PIN_2 };

SwitchState get_switch_state(GPIO_Pin sw) {
    GPIO_PinState switchState = HAL_GPIO_ReadPin(sw.port, sw.pin);
    if(switchState == GPIO_PIN_SET) return ON;
    return OFF;
}