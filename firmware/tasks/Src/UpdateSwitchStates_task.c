#include "ControlsLeader_Tasks.h"
#include "init.h"

void UpdateSwitchStatesCAN_task(void *argument) {

    while(1) {
        set_LED(X_LED2, GPIO_PIN_SET);
        vTaskDelay(pdMS_TO_TICKS(500));
        set_LED(X_LED2, GPIO_PIN_RESET);
        vTaskDelay(pdMS_TO_TICKS(500));
        //set_LED(AKSHAY_LED, get_switch_state(FWD_SW));
    }
}