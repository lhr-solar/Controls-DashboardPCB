#include "init.h"

void GPIO_Init() {
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();

    // Output Ports
    GPIO_InitTypeDef GPIOA_OUT_config = {
        .Mode = GPIO_MODE_OUTPUT_PP,
        .Pull = GPIO_NOPULL,
        .Pin  = PH_CAN_RX.pin | BPS_Fault_LED.pin
    };

    GPIO_InitTypeDef GPIOB_OUT_config = {
        .Mode = GPIO_MODE_OUTPUT_PP,
        .Pull = GPIO_NOPULL,
        .Pin  = CarCan_RX.pin | CarCan_TX.pin | PH_CAN_TX.pin | X_LED2.pin
    };

    GPIO_InitTypeDef GPIOC_OUT_config = {
        .Mode = GPIO_MODE_OUTPUT_PP,
        .Pull = GPIO_NOPULL,
        .Pin  = Controls_HB_LED.pin | LSOM_HB.pin | AKSHAY_LED.pin
    };

    // Input Ports
    GPIO_InitTypeDef GPIOA_IN_config = {
        .Mode = GPIO_MODE_INPUT,
        .Pull = GPIO_NOPULL,
        .Pin  = FWD_SW.pin | Ign_OFF.pin | Ign_MTR.pin
    };

    GPIO_InitTypeDef GPIOB_IN_config = {
        .Mode = GPIO_MODE_INPUT,
        .Pull = GPIO_NOPULL,
        .Pin  = PTT.pin | Ign_ARR.pin
    };

    GPIO_InitTypeDef GPIOC_IN_config = {
        .Mode = GPIO_MODE_INPUT,
        .Pull = GPIO_NOPULL,
        .Pin  = Neutral_Gear.pin | Regen_Enable.pin | Right_Blinker.pin |
                Left_Blinker.pin | Cruise_Enable.pin | Rev_SW.pin |
                Cruise_Set.pin | Regen_Active.pin | Hazard.pin
    };

    GPIO_InitTypeDef GPIOD_IN_config = {
        .Mode = GPIO_MODE_INPUT,
        .Pull = GPIO_NOPULL,
        .Pin  = Horn.pin
    };

    // Initialize outputs
    HAL_GPIO_Init(GPIOA, &GPIOA_OUT_config);
    HAL_GPIO_Init(GPIOB, &GPIOB_OUT_config);
    HAL_GPIO_Init(GPIOC, &GPIOC_OUT_config);

    //Set all output GPIO to low
    HAL_GPIO_WritePin(GPIOA, GPIOA_OUT_config.Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, GPIOB_OUT_config.Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOC, GPIOC_OUT_config.Pin, GPIO_PIN_RESET);

    HAL_GPIO_Init(GPIOA, &GPIOA_IN_config);
    HAL_GPIO_Init(GPIOB, &GPIOB_IN_config);
    HAL_GPIO_Init(GPIOC, &GPIOC_IN_config);
    HAL_GPIO_Init(GPIOD, &GPIOD_IN_config);
}