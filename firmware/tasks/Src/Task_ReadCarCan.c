#include "Tasks.h"
#include "init.h"
#include "Status_LEDs.h"
#include "Horn.h"
#include "Switches.h"

void ReadCarCAN_task(void *argument) {
    TickType_t xLastWakeTime = xTaskGetTickCount();

    while (1) {
        led_toggle(LSOM_HB_PORT, LSOM_HB_PIN);

        /**
         * @todo    implement fdcan3 rx driver here
         */

        vTaskDelayUntil(&xLastWakeTime, READ_CARCAN_TASK_DELAY_TICKS);
    }
}