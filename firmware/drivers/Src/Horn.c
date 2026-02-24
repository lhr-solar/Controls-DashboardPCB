#include "init.h"

const GPIO_Pin Horn_OUT = { GPIOB, GPIO_PIN_1 };

void Horn_GPIO_Init() {
    gpioPin_Init(Horn_OUT, GPIO_MODE_OUTPUT_PP);
}

void set_Horn(SwitchState state) {
    set_LED(AKSHAY_LED, state);
    HAL_GPIO_WritePin(Horn_OUT.port, Horn_OUT.pin, state);
}