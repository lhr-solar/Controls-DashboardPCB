#include "init.h"
#include "Switches.h"
#include "Status_LEDs.h"
#include "Horn.h"

// Switch state array index map:
// Index  | Switch
// -------|----------------
//   0    | Ign_ARR
//   1    | Ign_MTR
//   2    | Cruise_Enable
//   3    | Cruise_Set
//   4    | FWD_SW
//   5    | Neutral_Gear
//   6    | Rev_SW
//   7    | Hazard
//   8    | Left_Blinker
//   9    | Right_Blinker
//  10    | Horn
//  11    | PTT
//  12    | Regen_Enable
//  13    | Regen_Active
//  14    | BPS strobe

// Event group handle to store state bits
static uint32_t switch_bitmap = 0;


void switch_GPIO_init() {
	__HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
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

	
    switch_EXTI_Init(NEUTRAL_GEAR_PORT, NEUTRAL_GEAR_PIN, CL_MAX_PRIO);
    switch_EXTI_Init(IGN_OFF_PORT, IGN_OFF_PIN, CL_MAX_PRIO);
}

void gpioEXTI_Init(GPIO_TypeDef *port, uint16_t pin) {
	if(port == NULL) return;
    GPIO_InitTypeDef GPIO_init = {
        .Mode = GPIO_MODE_IT_RISING_FALLING,
        .Pull = GPIO_NOPULL,
        .Pin  = pin
    };

    HAL_GPIO_Init(port, &GPIO_init);
}

void switch_EXTI_Init(GPIO_TypeDef *port, uint16_t pin, uint32_t priority) {
	if(port == NULL) return;
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

uint32_t switch_read_all_inputs() {
    uint32_t newBits = 0;
	
    portENTER_CRITICAL();

    newBits |= (switch_get_state(IGN_ARR_PORT,       IGN_ARR_PIN)       == SWITCH_ON ? GET_MASK(SW_IGN_ARR)       : 0);
    newBits |= (switch_get_state(IGN_MTR_PORT,       IGN_MTR_PIN)       == SWITCH_ON ? GET_MASK(SW_IGN_MTR)       : 0);
    newBits |= (switch_get_state(CRUISE_ENABLE_PORT, CRUISE_ENABLE_PIN) == SWITCH_ON ? GET_MASK(SW_CRUISE_ENABLE) : 0);
    newBits |= (switch_get_state(CRUISE_SET_PORT,    CRUISE_SET_PIN)    == SWITCH_ON ? GET_MASK(SW_CRUISE_SET)    : 0);
    newBits |= (switch_get_state(FWD_SW_PORT,        FWD_SW_PIN)        == SWITCH_ON ? GET_MASK(SW_FWD)           : 0);
    newBits |= (switch_get_state(NEUTRAL_GEAR_PORT,  NEUTRAL_GEAR_PIN)  == SWITCH_ON ? GET_MASK(SW_NEUTRAL_GEAR)  : 0);
    newBits |= (switch_get_state(REV_SW_PORT,        REV_SW_PIN)        == SWITCH_ON ? GET_MASK(SW_REV)           : 0);
    newBits |= (switch_get_state(HAZARD_PORT,        HAZARD_PIN)        == SWITCH_ON ? GET_MASK(SW_HAZARD)        : 0);
    newBits |= (switch_get_state(LEFT_BLINKER_PORT,  LEFT_BLINKER_PIN)  == SWITCH_ON ? GET_MASK(SW_LEFT_BLINKER)  : 0);
    newBits |= (switch_get_state(RIGHT_BLINKER_PORT, RIGHT_BLINKER_PIN) == SWITCH_ON ? GET_MASK(SW_RIGHT_BLINKER) : 0);
    newBits |= (switch_get_state(HORN_PORT,          HORN_PIN)          == SWITCH_ON ? GET_MASK(SW_HORN)          : 0);
    newBits |= (switch_get_state(PTT_PORT,           PTT_PIN)           == SWITCH_ON ? GET_MASK(SW_PTT)           : 0);
    newBits |= (switch_get_state(REGEN_ENABLE_PORT,  REGEN_ENABLE_PIN)  == SWITCH_ON ? GET_MASK(SW_REGEN_ENABLE)  : 0);
    newBits |= (switch_get_state(REGEN_ACTIVE_PORT,  REGEN_ACTIVE_PIN)  == SWITCH_ON ? GET_MASK(SW_REGEN_ACTIVE)  : 0);

    switch_bitmap = newBits;
    portEXIT_CRITICAL();
	return switch_bitmap;
}

switch_state_t switch_get_state(GPIO_TypeDef *port, uint16_t pin) {
    return (HAL_GPIO_ReadPin(port, pin) == GPIO_PIN_SET) ? SWITCH_ON : SWITCH_OFF;
}


uint32_t switch_bitmap_setBit(state_bit_t bit, switch_state_t state) {
	if(bit >= SW_COUNT) return switch_bitmap;

	uint32_t mask = GET_MASK(bit);

	if(state == SWITCH_ON) switch_bitmap |= mask;
	else switch_bitmap &= ~(mask);

	return switch_bitmap;
}

uint32_t switch_bitmap_setAll(uint32_t bits) {
	switch_bitmap = bits;
	return switch_bitmap;
}

uint32_t switch_bitmap_read() {
	return switch_bitmap;
}



/**
 * @brief  EXTI callback fired on any configured external interrupt edge.
 *         Handles debounce, then updates LEDs based on switch states.
 *         - NEUTRAL_GEAR: updates PH_CAN_TX_LED, and CAR_CAN_TX_LED if FWD or REV is active.
 *         - IGN_OFF:      updates PH_CAN_RX_LED to mirror ignition off switch state.
 * 
 * @param  GPIO_Pin  Pin number that triggered the interrupt.
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    if (GPIO_Pin == NEUTRAL_GEAR_PIN) {
		led_set(AKSHAY_LED_PORT, AKSHAY_LED_PIN, switch_get_state(NEUTRAL_GEAR_PORT, NEUTRAL_GEAR_PIN));
        if (switch_get_state(FWD_SW_PORT, FWD_SW_PIN) == SWITCH_ON) {
        } else if (switch_get_state(REV_SW_PORT, REV_SW_PIN) == SWITCH_ON) {
        } else {
        }
    } else if (GPIO_Pin == IGN_OFF_PIN) {
		led_set(CONTROLS_HB_LED_PORT, CONTROLS_HB_LED_PIN, switch_get_state(IGN_OFF_PORT, IGN_OFF_PIN));
    }

	/**
	 * 
	 * @todo 	write Neautral Gear and Ign Off States to the switch states payload
	 * 
	*/
}

/**
 * @brief  EXTI line 0 interrupt handler. Routed to NEUTRAL_GEAR_PIN.
 */
void EXTI0_IRQHandler(void) {
    HAL_GPIO_EXTI_IRQHandler(NEUTRAL_GEAR_PIN);
}

/**
 * @brief  EXTI line 2 interrupt handler. Routed to IGN_OFF_PIN.
 */
void EXTI2_IRQHandler(void) {
    HAL_GPIO_EXTI_IRQHandler(IGN_OFF_PIN);
}