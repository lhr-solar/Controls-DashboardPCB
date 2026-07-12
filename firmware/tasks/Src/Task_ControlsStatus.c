#include "Tasks.h"
#include "CarCAN.h"
#include "CarCAN_can_msgs.h"
#include "init.h"
#include "Status_LEDs.h"
#include "Horn.h"
#include "Switches.h"


void ReadCarCAN_task(void *argument) {
    TickType_t xLastWakeTime = xTaskGetTickCount();

    while (1) {
        

        vTaskDelayUntil(&xLastWakeTime, READ_CARCAN_TASK_DELAY_TICKS);
    }
}