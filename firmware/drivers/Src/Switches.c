#include "init.h"

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

//[Ign_ARR - 0, Ign_MTR - 1, Cruise_Enable- 2, Cruise_Set - 3, FWD_SW - 4, Nuteral_Gear - 5, Rev_SW - 6, 
// hazard - 7, blinkerLeft - 8, blinker right - 9, horn - 10, ptt - 11, regen enable - 12, 
// regen active - 13, free bit - 14]
uint8_t switchStates[15] = {0};

void Switch_GPIO_Init() {
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();

    gpioPin_Init(FWD_SW, GPIO_MODE_INPUT);
    gpioPin_Init(Ign_OFF, GPIO_MODE_INPUT);
    gpioPin_Init(Ign_MTR, GPIO_MODE_INPUT);
    gpioPin_Init(Ign_ARR, GPIO_MODE_INPUT);
    gpioPin_Init(PTT, GPIO_MODE_INPUT);
    gpioPin_Init(Neutral_Gear, GPIO_MODE_INPUT);
    gpioPin_Init(Rev_SW, GPIO_MODE_INPUT);
    gpioPin_Init(Regen_Enable, GPIO_MODE_INPUT);
    gpioPin_Init(Right_Blinker, GPIO_MODE_INPUT);
    gpioPin_Init(Left_Blinker, GPIO_MODE_INPUT);
    gpioPin_Init(Cruise_Enable, GPIO_MODE_INPUT);
    gpioPin_Init(Cruise_Set, GPIO_MODE_INPUT);
    gpioPin_Init(Regen_Active, GPIO_MODE_INPUT);
    gpioPin_Init(Hazard, GPIO_MODE_INPUT);
    gpioPin_Init(Horn, GPIO_MODE_INPUT);

    
}

void EXTI_Init(GPIO_Pin sw, uint32_t priority) {
    gpioEXTI_Init(sw);
    uint32_t IRQn;
    uint32_t pin_index = __builtin_ctz(sw.pin);

    if (pin_index <= 4) {
        IRQn = (EXTI0_IRQn + pin_index);
    } else if (pin_index <= 9) {
        IRQn = EXTI9_5_IRQn;
    } else {
        IRQn = EXTI15_10_IRQn;
    }
 
    HAL_NVIC_SetPriority(IRQn, priority, 0);
    HAL_NVIC_EnableIRQ(IRQn);
}

void PollAllSwitchStates() {
    //[Ign_ARR - 0, Ign_MTR - 1, Cruise_Enable- 2, Cruise_Set - 3, FWD_SW - 4, Nuteral_Gear - 5, Rev_SW - 6, 
    // hazard - 7, blinkerLeft - 8, blinker right - 9, horn - 10, ptt - 11, regen enable - 12, 
    // regen active - 13, free bit - 14]

    switchStates[0] = (get_switch_state(Ign_ARR) == ON ? 1 : 0);
    switchStates[1] = (get_switch_state(Ign_MTR) == ON ? 1 : 0);
    switchStates[2] = (get_switch_state(Cruise_Enable) == ON ? 1 : 0);
    switchStates[3] = (get_switch_state(Cruise_Set) == ON ? 1 : 0);
    switchStates[4] = (get_switch_state(FWD_SW) == ON ? 1 : 0);
    switchStates[5] = (get_switch_state(Neutral_Gear) == ON ? 1 : 0);
    switchStates[6] = (get_switch_state(Rev_SW) == ON ? 1 : 0);
    switchStates[7] = (get_switch_state(Hazard) == ON ? 1 : 0);
    switchStates[8] = (get_switch_state(Left_Blinker) == ON ? 1 : 0);
    switchStates[9] = (get_switch_state(Right_Blinker) == ON ? 1 : 0);
    switchStates[10] = (get_switch_state(Horn) == ON ? 1 : 0);
    switchStates[11] = (get_switch_state(PTT) == ON ? 1 : 0);
    switchStates[12] = (get_switch_state(Regen_Enable) == ON ? 1 : 0);
    switchStates[13] = (get_switch_state(Regen_Active) == ON ? 1 : 0);  

}

SwitchState get_switch_state(GPIO_Pin sw) {
    GPIO_PinState switchState = HAL_GPIO_ReadPin(sw.port, sw.pin);
    if(switchState == GPIO_PIN_SET) return ON;
    return OFF;
}