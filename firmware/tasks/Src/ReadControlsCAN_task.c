#include "ControlsLeader_Tasks.h"
#include "init.h"

void ReadControlsCAN_task(void *argument) {

    while(1) {
        set_LED(PH_CAN_RX, GPIO_PIN_SET);
        HAL_Delay(500);
        set_LED(PH_CAN_RX, GPIO_PIN_RESET);
        HAL_Delay(500);
    }
}