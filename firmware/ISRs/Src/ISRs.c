#include "init.h"


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {

    vTaskDelay(pdMS_TO_TICKS(250));
    if (GPIO_Pin == Neutral_Gear.pin) {
        set_LED(PH_CAN_TX_LED, get_switch_state(Neutral_Gear));
        if(get_switch_state(FWD_SW) == ON) {
            set_LED(CarCAN_TX_LED, ON);
        } else if(get_switch_state(Rev_SW) == ON) {
            set_LED(CarCAN_TX_LED, ON);
        } else { 
            set_LED(PH_CAN_TX_LED, get_switch_state(Neutral_Gear));
        }
    } else if (GPIO_Pin == Ign_OFF.pin) {
        set_LED(PH_CAN_RX_LED, get_switch_state(Ign_OFF));
    }

}


void EXTI0_IRQHandler(void) {
    HAL_GPIO_EXTI_IRQHandler(Neutral_Gear.pin);
}

void EXTI2_IRQHandler(void) {
    HAL_GPIO_EXTI_IRQHandler(Ign_OFF.pin);
}


