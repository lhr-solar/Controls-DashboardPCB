#include "init.h"

//Port A - out
const GPIO_Pin PH_CAN_RX_LED       = { GPIOA, GPIO_PIN_10 };

//Port B - out
const GPIO_Pin CarCAN_RX_LED       = { GPIOB, GPIO_PIN_3  };
const GPIO_Pin CarCAN_TX_LED       = { GPIOB, GPIO_PIN_5  };
const GPIO_Pin PH_CAN_TX_LED       = { GPIOB, GPIO_PIN_4  };
const GPIO_Pin X_LED2          = { GPIOB, GPIO_PIN_14 };
const GPIO_Pin Controls_HB_LED = { GPIOB, GPIO_PIN_10 };
const GPIO_Pin AKSHAY_LED      = { GPIOB, GPIO_PIN_11 };

//Port C - out
const GPIO_Pin BPS_Fault_LED   = { GPIOC, GPIO_PIN_1  };
const GPIO_Pin LSOM_HB         = { GPIOC, GPIO_PIN_3  };

void LED_GPIO_Init() {
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();

    gpioPin_Init(PH_CAN_RX_LED, GPIO_MODE_OUTPUT_PP);
    gpioPin_Init(CarCAN_RX_LED, GPIO_MODE_OUTPUT_PP);
    gpioPin_Init(CarCAN_TX_LED, GPIO_MODE_OUTPUT_PP);
    gpioPin_Init(PH_CAN_TX_LED, GPIO_MODE_OUTPUT_PP);
    gpioPin_Init(X_LED2, GPIO_MODE_OUTPUT_PP);
    gpioPin_Init(Controls_HB_LED, GPIO_MODE_OUTPUT_PP);
    gpioPin_Init(AKSHAY_LED, GPIO_MODE_OUTPUT_PP);
    gpioPin_Init(BPS_Fault_LED, GPIO_MODE_OUTPUT_PP);
    gpioPin_Init(LSOM_HB, GPIO_MODE_OUTPUT_PP);
}

void set_LED(GPIO_Pin LED, GPIO_PinState state) {
    HAL_GPIO_WritePin(LED.port, LED.pin, state);
}

void toggle_LED(GPIO_Pin LED) {
    HAL_GPIO_TogglePin(LED.port, LED.pin);
}

void flash_them() {
    HAL_GPIO_TogglePin(PH_CAN_RX_LED.port,       PH_CAN_RX_LED.pin);
    HAL_GPIO_TogglePin(BPS_Fault_LED.port,   BPS_Fault_LED.pin);

    HAL_GPIO_TogglePin(CarCAN_RX_LED.port,       CarCAN_RX_LED.pin);
    HAL_GPIO_TogglePin(CarCAN_TX_LED.port,       CarCAN_TX_LED.pin);
    HAL_GPIO_TogglePin(PH_CAN_TX_LED.port,       PH_CAN_TX_LED.pin);
    HAL_GPIO_TogglePin(X_LED2.port,          X_LED2.pin);

    HAL_GPIO_TogglePin(Controls_HB_LED.port, Controls_HB_LED.pin);
    HAL_GPIO_TogglePin(LSOM_HB.port,         LSOM_HB.pin);
    HAL_GPIO_TogglePin(AKSHAY_LED.port,      AKSHAY_LED.pin);

    HAL_Delay(250);    
    
    HAL_GPIO_TogglePin(PH_CAN_RX_LED.port,       PH_CAN_RX_LED.pin);
    HAL_GPIO_TogglePin(BPS_Fault_LED.port,   BPS_Fault_LED.pin);

    HAL_GPIO_TogglePin(CarCAN_RX_LED.port,       CarCAN_RX_LED.pin);
    HAL_GPIO_TogglePin(CarCAN_TX_LED.port,       CarCAN_TX_LED.pin);
    HAL_GPIO_TogglePin(PH_CAN_TX_LED.port,       PH_CAN_TX_LED.pin);
    HAL_GPIO_TogglePin(X_LED2.port,          X_LED2.pin);

    HAL_GPIO_TogglePin(Controls_HB_LED.port, Controls_HB_LED.pin);
    HAL_GPIO_TogglePin(LSOM_HB.port,         LSOM_HB.pin);
    HAL_GPIO_TogglePin(AKSHAY_LED.port,      AKSHAY_LED.pin);

    HAL_Delay(250);    

}

void RainbowLED() {
    for(int i = 0; i < 2; i++) {
        HAL_GPIO_TogglePin(LSOM_HB.port,          LSOM_HB.pin);
        HAL_Delay(50);

        HAL_GPIO_TogglePin(AKSHAY_LED.port,       AKSHAY_LED.pin);
        HAL_Delay(50);

        HAL_GPIO_TogglePin(Controls_HB_LED.port,  Controls_HB_LED.pin);
        HAL_Delay(50);

        HAL_GPIO_TogglePin(BPS_Fault_LED.port,    BPS_Fault_LED.pin);
        HAL_Delay(50);

        HAL_GPIO_TogglePin(PH_CAN_TX_LED.port,    PH_CAN_TX_LED.pin);
        HAL_Delay(50);

        HAL_GPIO_TogglePin(PH_CAN_RX_LED.port,    PH_CAN_RX_LED.pin);
        HAL_Delay(50);

        HAL_GPIO_TogglePin(CarCAN_TX_LED.port,    CarCAN_TX_LED.pin);
        HAL_Delay(50);

        HAL_GPIO_TogglePin(CarCAN_RX_LED.port,    CarCAN_RX_LED.pin);
        HAL_Delay(50);

        HAL_GPIO_TogglePin(X_LED2.port,           X_LED2.pin);
        HAL_Delay(50);
    }
}



