#include "init.h"
#include "Status_LEDs.h"
#include "Horn.h"
#include "Switches.h"

void horn_GPIO_init() {
    gpioPin_Init(HORN_OUT_PORT, HORN_OUT_PIN, GPIO_MODE_OUTPUT_PP);
}

void horn_set(switch_state_t state) {
    led_set(AKSHAY_LED_PORT, AKSHAY_LED_PIN, state);
    HAL_GPIO_WritePin(HORN_OUT_PORT, HORN_OUT_PIN, state);
}