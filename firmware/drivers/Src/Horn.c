#include "init.h"
#include "Status_LEDs.h"
#include "Horn.h"
#include "Switches.h"

void horn_GPIO_init() {
    __HAL_RCC_GPIOB_CLK_ENABLE();
    gpioPin_Init(HORN_OUT_PORT, HORN_OUT_PIN, GPIO_MODE_OUTPUT_PP);
}

void horn_set(switch_state_t state) {
    HAL_GPIO_WritePin(HORN_OUT_PORT, HORN_OUT_PIN, state);
}