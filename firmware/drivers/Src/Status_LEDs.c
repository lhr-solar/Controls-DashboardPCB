#include "init.h"
#include "Status_LEDs.h"

static bool bps_strobe = false;

void led_GPIO_init() {
	__HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    gpioPin_Init(PH_CAN_RX_LED_PORT,  PH_CAN_RX_LED_PIN,  GPIO_MODE_OUTPUT_PP);
    gpioPin_Init(CAR_CAN_RX_LED_PORT, CAR_CAN_RX_LED_PIN, GPIO_MODE_OUTPUT_PP);
    gpioPin_Init(CAR_CAN_TX_LED_PORT, CAR_CAN_TX_LED_PIN, GPIO_MODE_OUTPUT_PP);
    gpioPin_Init(PH_CAN_TX_LED_PORT,  PH_CAN_TX_LED_PIN,  GPIO_MODE_OUTPUT_PP);
    gpioPin_Init(X_LED2_PORT,         X_LED2_PIN,         GPIO_MODE_OUTPUT_PP);
    gpioPin_Init(CONTROLS_HB_LED_PORT,CONTROLS_HB_LED_PIN,GPIO_MODE_OUTPUT_PP);
    gpioPin_Init(AKSHAY_LED_PORT,     AKSHAY_LED_PIN,     GPIO_MODE_OUTPUT_PP);
    gpioPin_Init(BPS_FAULT_LED_PORT,  BPS_FAULT_LED_PIN,  GPIO_MODE_OUTPUT_PP);
    gpioPin_Init(LSOM_HB_PORT,        LSOM_HB_PIN,        GPIO_MODE_OUTPUT_PP);
}

void led_set(GPIO_TypeDef *port, uint16_t pin, GPIO_PinState state) {
	if(port == NULL) return;
    HAL_GPIO_WritePin(port, pin, state);
}

void led_toggle(GPIO_TypeDef *port, uint16_t pin) {
	if(port == NULL) return;
    HAL_GPIO_TogglePin(port, pin);
}

void flash_them() {
    HAL_GPIO_TogglePin(PH_CAN_RX_LED_PORT,   PH_CAN_RX_LED_PIN);
    HAL_GPIO_TogglePin(BPS_FAULT_LED_PORT,    BPS_FAULT_LED_PIN);
    HAL_GPIO_TogglePin(CAR_CAN_RX_LED_PORT,   CAR_CAN_RX_LED_PIN);
    HAL_GPIO_TogglePin(CAR_CAN_TX_LED_PORT,   CAR_CAN_TX_LED_PIN);
    HAL_GPIO_TogglePin(PH_CAN_TX_LED_PORT,    PH_CAN_TX_LED_PIN);
    HAL_GPIO_TogglePin(X_LED2_PORT,           X_LED2_PIN);
    HAL_GPIO_TogglePin(CONTROLS_HB_LED_PORT,  CONTROLS_HB_LED_PIN);
    HAL_GPIO_TogglePin(LSOM_HB_PORT,          LSOM_HB_PIN);
    HAL_GPIO_TogglePin(AKSHAY_LED_PORT,       AKSHAY_LED_PIN);
}


void updateBPS_strobe(uint8_t bps_fault) {
	if(bps_fault == 0) bps_strobe = false;
	else bps_strobe = true;
		/**
		 * 
		 * @todo 	maybe do more stuff here
		 * 
		 */
}