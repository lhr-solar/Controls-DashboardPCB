#include "init.h"


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {

    if (GPIO_Pin == FWD_SW.pin) {
        set_LED(PH_CAN_TX_LED, get_switch_state(FWD_SW));
    } else if (GPIO_Pin == Ign_OFF.pin) {
        set_LED(PH_CAN_RX_LED, get_switch_state(Ign_OFF));
    } else if (GPIO_Pin == Ign_MTR.pin) {
        set_LED(AKSHAY_LED, get_switch_state(Ign_MTR));
    }

}


void EXTI0_IRQHandler(void) {
    toggle_LED(AKSHAY_LED);
    HAL_GPIO_EXTI_IRQHandler(FWD_SW.pin);
    toggle_LED(AKSHAY_LED);
}