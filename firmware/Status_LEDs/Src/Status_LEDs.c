#include "Status_LEDs.h"

void set_LED(GPIO_Pin LED, GPIO_PinState state) {
    HAL_GPIO_WritePin(LED.port, LED.pin, state);
}

void flash_them() {
    while (1) {
        HAL_GPIO_TogglePin(PH_CAN_RX.port,       PH_CAN_RX.pin);
        HAL_GPIO_TogglePin(BPS_Fault_LED.port,   BPS_Fault_LED.pin);

        HAL_GPIO_TogglePin(CarCan_RX.port,       CarCan_RX.pin);
        HAL_GPIO_TogglePin(CarCan_TX.port,       CarCan_TX.pin);
        HAL_GPIO_TogglePin(PH_CAN_TX.port,       PH_CAN_TX.pin);
        HAL_GPIO_TogglePin(X_LED2.port,          X_LED2.pin);

        HAL_GPIO_TogglePin(Controls_HB_LED.port, Controls_HB_LED.pin);
        HAL_GPIO_TogglePin(LSOM_HB.port,         LSOM_HB.pin);
        HAL_GPIO_TogglePin(AKSHAY_LED.port,      AKSHAY_LED.pin);

        HAL_Delay(500);
    }

}



