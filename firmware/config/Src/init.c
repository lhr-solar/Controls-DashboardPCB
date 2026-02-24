#include "init.h"
#include "stm32xx_hal.h"

void GPIO_Init() {
    LED_GPIO_Init();
    Switch_GPIO_Init();
    Horn_GPIO_Init();
}

void gpioPin_Init(GPIO_Pin pin, uint32_t mode) {
    GPIO_InitTypeDef GPIO_init = {
        .Mode = mode,
        .Pull = GPIO_NOPULL,
        .Pin  = pin.pin
    };

    HAL_GPIO_Init(pin.port, &GPIO_init);
    if(mode != GPIO_MODE_OUTPUT_PP) HAL_GPIO_WritePin(pin.port, pin.pin, GPIO_PIN_RESET);
}

void gpioEXTI_Init(GPIO_Pin pin) {
    GPIO_InitTypeDef GPIO_init = {
        .Mode = GPIO_MODE_IT_RISING_FALLING,
        .Pull = GPIO_NOPULL,
        .Pin  = pin.pin
    };

    HAL_GPIO_Init(pin.port, &GPIO_init);
}