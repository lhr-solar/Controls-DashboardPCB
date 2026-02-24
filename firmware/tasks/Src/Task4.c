#include "ControlsLeader_Tasks.h"
#include "init.h"

void ReadControlsCAN_task(void *argument) {

    while(1) {
        set_LED(Controls_HB_LED, GPIO_PIN_SET);
        vTaskDelay(pdMS_TO_TICKS(500));
        set_LED(Controls_HB_LED, GPIO_PIN_RESET);
        vTaskDelay(pdMS_TO_TICKS(500));
    }

}