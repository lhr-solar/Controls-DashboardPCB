#include "init.h"

void GPIO_Init() {
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_SYSCFG_CLK_ENABLE();

    //Output Ports
    GPIO_InitTypeDef GPIOA_OUT_config = {
        .Mode = GPIO_MODE_OUTPUT_PP,
        .Pull = GPIO_NOPULL,
        .Pin  = PH_CAN_RX_LED.pin
    };

    GPIO_InitTypeDef GPIOB_OUT_config = {
        .Mode = GPIO_MODE_OUTPUT_PP,
        .Pull = GPIO_NOPULL,
        .Pin  = Controls_HB_LED.pin | CarCAN_RX_LED.pin | 
                CarCAN_TX_LED.pin | PH_CAN_TX_LED.pin | X_LED2.pin
    };

    GPIO_InitTypeDef GPIOC_OUT_config = {
        .Mode = GPIO_MODE_OUTPUT_PP,
        .Pull = GPIO_NOPULL,
        .Pin  = LSOM_HB.pin | AKSHAY_LED.pin | BPS_Fault_LED.pin
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
        .Pin  = PTT.pin
    };

    GPIO_InitTypeDef GPIOC_IN_config = {
        .Mode = GPIO_MODE_INPUT,
        .Pull = GPIO_NOPULL,
        .Pin  = Regen_Enable.pin | Right_Blinker.pin |
                Left_Blinker.pin | Cruise_Enable.pin |
                Cruise_Set.pin | Regen_Active.pin | Hazard.pin
    };

    GPIO_InitTypeDef GPIOD_IN_config = {
        .Mode = GPIO_MODE_INPUT,
        .Pull = GPIO_NOPULL,
        .Pin  = Horn.pin
    };

    //Ignition & gear Switch Init
    //GPIOA
    GPIO_InitTypeDef EXT_InitA = {
        .Mode = GPIO_MODE_IT_RISING_FALLING,
        .Pull = GPIO_NOPULL,
        .Pin  = FWD_SW.pin | Ign_OFF.pin | Ign_MTR.pin
    };

    //GPIOB
    GPIO_InitTypeDef EXT_InitB = {
        .Mode = GPIO_MODE_IT_RISING_FALLING,
        .Pull = GPIO_NOPULL,
        .Pin  = Ign_ARR.pin
    };

    //GPIOC
    GPIO_InitTypeDef EXT_InitC = {
        .Mode = GPIO_MODE_IT_RISING_FALLING,
        .Pull = GPIO_NOPULL,
        .Pin  = Neutral_Gear.pin | Rev_SW.pin
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

    //GPIO With Interrupts Init
    HAL_GPIO_Init(GPIOA, &EXT_InitA);
    HAL_GPIO_Init(GPIOB, &EXT_InitB);
    HAL_GPIO_Init(GPIOC, &EXT_InitC);

     /* Give EXTI0 a higher hardware priority than FreeRTOS max syscall priority
         so it is not masked by the RTOS BASEPRI. */
    HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0); //FWD_SW + Ign_ARR + Neutral_Gear
    HAL_NVIC_EnableIRQ(EXTI0_IRQn);

    // HAL_NVIC_SetPriority(EXTI1_IRQn, 5, 0); //Ign_MTR
    // HAL_NVIC_EnableIRQ(EXTI1_IRQn);

    // HAL_NVIC_SetPriority(EXTI2_IRQn, 5, 0); //Ign_OFF
    // HAL_NVIC_EnableIRQ(EXTI2_IRQn);

    // HAL_NVIC_SetPriority(EXTI9_5_IRQn, 5, 0); //Rev_SW
    // HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
}