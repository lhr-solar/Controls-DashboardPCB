#include "FreeRTOS.h"
#include "timers.h"
#include "init.h"
#include "Switches.h"
#include "Status_LEDs.h"
#include "Horn.h"


#define EXTI_TIMER_PERIOD 	pdMS_TO_TICKS(1000)  // 200 ms

static StaticTimer_t xTimerBuffer_GearSW;
static TimerHandle_t xDebounceTimer_GearSW = NULL;
static StaticTimer_t xTimerBuffer_IgnitionSW;
static TimerHandle_t xDebounceTimer_IgnitionSW = NULL;


// Bit map to store all switch states as a payload
static uint32_t switch_bitmap = 0;


static void vTimerCallback_GearSW(TimerHandle_t xTimer);
static void vTimerCallback_IgnitionSW(TimerHandle_t xTimer);

void switch_init() {
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

	
    switch_EXTI_Init(NEUTRAL_GEAR_PORT, NEUTRAL_GEAR_PIN, SWITCH_MAX_PRIO);
    switch_EXTI_Init(IGN_ARR_PORT, IGN_ARR_PIN, SWITCH_MAX_PRIO);

	//Timer for 3-state Gear switch debounce... used in callbacks later in the file
	xDebounceTimer_GearSW = xTimerCreateStatic(
        "EXTI Callback Delay",
        EXTI_TIMER_PERIOD,
        pdFALSE,
        (void *)0,
        vTimerCallback_GearSW,
        &xTimerBuffer_GearSW
    );

	//Timer for 3-state Ignition switch debounce... used in callbacks later in the file
	xDebounceTimer_IgnitionSW = xTimerCreateStatic(
        "EXTI Callback Delay",
        EXTI_TIMER_PERIOD,
        pdFALSE,
        (void *)0,
        vTimerCallback_IgnitionSW,
        &xTimerBuffer_IgnitionSW
    );

	switch_bitmap_setBit(SW_IGN_OFF, switch_get_state(SW_IGN_OFF));
	switch_bitmap_setBit(SW_NEUTRAL_GEAR, switch_get_state(SW_NEUTRAL_GEAR));

}

void gpioEXTI_Init(GPIO_TypeDef *port, uint16_t pin) {
	if(port == NULL) return;
	gpioPin_Init(port, pin, GPIO_MODE_IT_RISING_FALLING);
}

void switch_EXTI_Init(GPIO_TypeDef *port, uint16_t pin, uint32_t NVIC_GPIO_EXT_PRIORITY) {
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
	
    HAL_NVIC_SetPriority(IRQn, NVIC_GPIO_EXT_PRIORITY, 0);
    HAL_NVIC_EnableIRQ(IRQn);
}

uint32_t switch_read_all_inputs() {
    for (int i = 0; i < SW_COUNT; i++) {
        switch_bitmap_setBit(i, switch_get_state(i));
    }
    return switch_bitmap;
}

switch_state_t switch_get_state(switch_bit_t sw) {
	switch(sw) {
		case SW_IGN_OFF:       return HAL_GPIO_ReadPin(IGN_OFF_PORT,        IGN_OFF_PIN)        == GPIO_PIN_SET ? SWITCH_OFF : SWITCH_ON;
		case SW_IGN_ARR:       return HAL_GPIO_ReadPin(IGN_ARR_PORT,        IGN_ARR_PIN)        == GPIO_PIN_SET ? SWITCH_OFF : SWITCH_ON;
		case SW_IGN_MTR:       return HAL_GPIO_ReadPin(IGN_MTR_PORT,        IGN_MTR_PIN)        == GPIO_PIN_SET ? SWITCH_OFF : SWITCH_ON;
		case SW_FWD:           return HAL_GPIO_ReadPin(FWD_SW_PORT,         FWD_SW_PIN)         == GPIO_PIN_SET ? SWITCH_OFF : SWITCH_ON;
		case SW_NEUTRAL_GEAR:  return HAL_GPIO_ReadPin(NEUTRAL_GEAR_PORT,   NEUTRAL_GEAR_PIN)   == GPIO_PIN_SET ? SWITCH_OFF : SWITCH_ON;
		case SW_REV:           return HAL_GPIO_ReadPin(REV_SW_PORT,         REV_SW_PIN)         == GPIO_PIN_SET ? SWITCH_OFF : SWITCH_ON;
		case SW_CRUISE_ENABLE: return HAL_GPIO_ReadPin(CRUISE_ENABLE_PORT,  CRUISE_ENABLE_PIN)  == GPIO_PIN_SET ? SWITCH_OFF : SWITCH_ON;
		case SW_CRUISE_SET:    return HAL_GPIO_ReadPin(CRUISE_SET_PORT,     CRUISE_SET_PIN)     == GPIO_PIN_SET ? SWITCH_OFF : SWITCH_ON;
		case SW_HAZARD:        return HAL_GPIO_ReadPin(HAZARD_PORT,         HAZARD_PIN)         == GPIO_PIN_SET ? SWITCH_OFF : SWITCH_ON;
		case SW_LEFT_BLINKER:  return HAL_GPIO_ReadPin(LEFT_BLINKER_PORT,   LEFT_BLINKER_PIN)   == GPIO_PIN_SET ? SWITCH_OFF : SWITCH_ON;
		case SW_RIGHT_BLINKER: return HAL_GPIO_ReadPin(RIGHT_BLINKER_PORT,  RIGHT_BLINKER_PIN)  == GPIO_PIN_SET ? SWITCH_OFF : SWITCH_ON;
		case SW_HORN:          return HAL_GPIO_ReadPin(HORN_PORT,           HORN_PIN)           == GPIO_PIN_SET ? SWITCH_OFF : SWITCH_ON;
		case SW_PTT:           return HAL_GPIO_ReadPin(PTT_PORT,            PTT_PIN)            == GPIO_PIN_SET ? SWITCH_OFF : SWITCH_ON;
		case SW_REGEN_ENABLE:  return HAL_GPIO_ReadPin(REGEN_ENABLE_PORT,   REGEN_ENABLE_PIN)   == GPIO_PIN_SET ? SWITCH_OFF : SWITCH_ON;
		case SW_REGEN_ACTIVE:  return HAL_GPIO_ReadPin(REGEN_ACTIVE_PORT,   REGEN_ACTIVE_PIN)   == GPIO_PIN_SET ? SWITCH_OFF : SWITCH_ON;
		default:               return SWITCH_ON;
	}
}


uint32_t switch_bitmap_setBit(switch_bit_t bit, switch_state_t state) {
	if(bit >= SW_COUNT) return switch_bitmap;

	uint32_t mask = GET_MASK(bit);

	portENTER_CRITICAL();
	if(state == SWITCH_ON) switch_bitmap |= mask;
	else switch_bitmap &= ~(mask);
	portEXIT_CRITICAL();

	return switch_bitmap;
}

void switch_bitmap_setAll(uint32_t bits) {
	portENTER_CRITICAL();
	switch_bitmap = bits;
	portEXIT_CRITICAL();
}

uint32_t switch_bitmap_read() {
	return switch_bitmap;
}

/**
 * @brief  Timer callback fired after debounce delay
 * 			
 * 		Updates LEDs based on switch states.
 *         - NEUTRAL_GEAR
 * 		   - FWD
 * 		   - REV
 * 
 * @param xTimer 	TimerHandler required for callback
 */
static void vTimerCallback_GearSW(TimerHandle_t xTimer) {
    switch_bitmap_setBit(SW_FWD,          switch_get_state(SW_FWD));
    switch_bitmap_setBit(SW_REV,          switch_get_state(SW_REV));
    switch_bitmap_setBit(SW_NEUTRAL_GEAR, switch_get_state(SW_NEUTRAL_GEAR));

}

/**
 * @brief  Timer callback fired after debounce delay
 * 			
 * 		Updates LEDs based on switch states.
 *         - IGN_OFF
 * 		   - IGN_ARR
 * 		   - IGN_MTR
 * 
 * @param xTimer 	TimerHandler required for callback
 */
static void vTimerCallback_IgnitionSW(TimerHandle_t xTimer) {
	switch_bitmap_setBit(SW_IGN_OFF, switch_get_state(SW_IGN_OFF));
	switch_bitmap_setBit(SW_IGN_ARR, switch_get_state(SW_IGN_ARR));
	switch_bitmap_setBit(SW_IGN_MTR, switch_get_state(SW_IGN_MTR));

}

/**
 * @brief  EXTI callback fired on any configured external interrupt edge.
 *         Handles debounce, then waits for timer callback
 * 
 * @param  GPIO_Pin  Pin number that triggered the interrupt.
 */
void HAL_GPIO_EXTI_GearSW_Callback(uint16_t GPIO_Pin) {
    UNUSED(GPIO_Pin);
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xTimerStartFromISR(xDebounceTimer_GearSW, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

/**
 * @brief  EXTI callback fired on any configured external interrupt edge.
 *         Handles debounce, then waits for timer callback
 * 
 * @param  GPIO_Pin  Pin number that triggered the interrupt.
 */
void HAL_GPIO_EXTI_IgnitionSW_Callback(uint16_t GPIO_Pin) {
    UNUSED(GPIO_Pin);
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xTimerStartFromISR(xDebounceTimer_IgnitionSW, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    if (GPIO_Pin == NEUTRAL_GEAR_PIN) HAL_GPIO_EXTI_GearSW_Callback(GPIO_Pin);
    else if (GPIO_Pin == IGN_ARR_PIN) HAL_GPIO_EXTI_IgnitionSW_Callback(GPIO_Pin);
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
    HAL_GPIO_EXTI_IRQHandler(IGN_ARR_PIN);
}