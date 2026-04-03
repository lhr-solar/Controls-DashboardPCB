#include "init.h"
#include "Switches.h"

void switch_GPIO_init() {
    gpioPin_Init(FWD_SW_PORT,         FWD_SW_PIN,         GPIO_MODE_INPUT);
    gpioPin_Init(IGN_OFF_PORT,        IGN_OFF_PIN,        GPIO_MODE_INPUT);
    gpioPin_Init(IGN_MTR_PORT,        IGN_MTR_PIN,        GPIO_MODE_INPUT);
    gpioPin_Init(IGN_ARR_PORT,        IGN_ARR_PIN,        GPIO_MODE_INPUT);
    gpioPin_Init(PTT_PORT,            PTT_PIN,            GPIO_MODE_INPUT);
    gpioPin_Init(NEUTRAL_GEAR_PORT,   NEUTRAL_GEAR_PIN,   GPIO_MODE_INPUT);
    gpioPin_Init(REV_SW_PORT,         REV_SW_PIN,         GPIO_MODE_INPUT);
    gpioPin_Init(REGEN_ENABLE_PORT,   REGEN_ENABLE_PIN,   GPIO_MODE_INPUT);
    gpioPin_Init(RIGHT_BLINKER_PORT,  RIGHT_BLINKER_PIN,  GPIO_MODE_INPUT);
    gpioPin_Init(LEFT_BLINKER_PORT,   LEFT_BLINKER_PIN,   GPIO_MODE_INPUT);
    gpioPin_Init(CRUISE_ENABLE_PORT,  CRUISE_ENABLE_PIN,  GPIO_MODE_INPUT);
    gpioPin_Init(CRUISE_SET_PORT,     CRUISE_SET_PIN,     GPIO_MODE_INPUT);
    gpioPin_Init(REGEN_ACTIVE_PORT,   REGEN_ACTIVE_PIN,   GPIO_MODE_INPUT);
    gpioPin_Init(HAZARD_PORT,         HAZARD_PIN,         GPIO_MODE_INPUT);
    gpioPin_Init(HORN_PORT,           HORN_PIN,           GPIO_MODE_INPUT);
}

void switch_EXTI_Init(GPIO_TypeDef *port, uint16_t pin, uint32_t priority) {
    gpioEXTI_Init(port, pin);

    uint32_t IRQn;
    uint32_t pin_index = __builtin_ctz(pin);

    if (pin_index <= INTERRUPT_0_TO_4_BOUND) {
        IRQn = EXTI0_IRQn + pin_index;
    } else if (pin_index <= INTERRUPT_5_TO_9_BOUND) {
        IRQn = EXTI9_5_IRQn;
    } else {
        IRQn = EXTI15_10_IRQn;
    }
	
    HAL_NVIC_SetPriority(IRQn, priority, 0);
    HAL_NVIC_EnableIRQ(IRQn);
}

void switch_poll_all_inputs(uint8_t* payload) {
    payload[SW_IGN_ARR]       = (switch_get_state(IGN_ARR_PORT,       IGN_ARR_PIN)       == ON ? 1 : 0);
    payload[SW_IGN_MTR]       = (switch_get_state(IGN_MTR_PORT,       IGN_MTR_PIN)       == ON ? 1 : 0);
    payload[SW_CRUISE_ENABLE] = (switch_get_state(CRUISE_ENABLE_PORT, CRUISE_ENABLE_PIN) == ON ? 1 : 0);
    payload[SW_CRUISE_SET]    = (switch_get_state(CRUISE_SET_PORT,    CRUISE_SET_PIN)    == ON ? 1 : 0);
    payload[SW_FWD]           = (switch_get_state(FWD_SW_PORT,        FWD_SW_PIN)        == ON ? 1 : 0);
    payload[SW_NEUTRAL_GEAR]  = (switch_get_state(NEUTRAL_GEAR_PORT,  NEUTRAL_GEAR_PIN)  == ON ? 1 : 0);
    payload[SW_REV]           = (switch_get_state(REV_SW_PORT,        REV_SW_PIN)        == ON ? 1 : 0);
    payload[SW_HAZARD]        = (switch_get_state(HAZARD_PORT,        HAZARD_PIN)        == ON ? 1 : 0);
    payload[SW_LEFT_BLINKER]  = (switch_get_state(LEFT_BLINKER_PORT,  LEFT_BLINKER_PIN)  == ON ? 1 : 0);
    payload[SW_RIGHT_BLINKER] = (switch_get_state(RIGHT_BLINKER_PORT, RIGHT_BLINKER_PIN) == ON ? 1 : 0);
    payload[SW_HORN]          = (switch_get_state(HORN_PORT,          HORN_PIN)          == ON ? 1 : 0);
    payload[SW_PTT]           = (switch_get_state(PTT_PORT,           PTT_PIN)           == ON ? 1 : 0);
    payload[SW_REGEN_ENABLE]  = (switch_get_state(REGEN_ENABLE_PORT,  REGEN_ENABLE_PIN)  == ON ? 1 : 0);
    payload[SW_REGEN_ACTIVE]  = (switch_get_state(REGEN_ACTIVE_PORT,  REGEN_ACTIVE_PIN)  == ON ? 1 : 0);
}

switch_state_t switch_get_state(GPIO_TypeDef *port, uint16_t pin) {
    return (HAL_GPIO_ReadPin(port, pin) == GPIO_PIN_SET) ? ON : OFF;
}