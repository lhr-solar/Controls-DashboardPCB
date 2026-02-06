#include "ControlsLeader_Tasks.h"
#include "init.h"


void ReadCarCAN_task(void *argument) {

    while(1) {
        set_LED(LSOM_HB, GPIO_PIN_SET);
        HAL_Delay(500);
        set_LED(LSOM_HB, GPIO_PIN_RESET);
        HAL_Delay(500);
    }
}