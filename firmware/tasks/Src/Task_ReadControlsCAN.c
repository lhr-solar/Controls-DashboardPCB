#include "Tasks.h"
#include "init.h"
#include "Status_LEDs.h"
#include "Horn.h"
#include "Switches.h"

void ReadControlsCAN_task(void *argument) {
    TickType_t xLastWakeTime = xTaskGetTickCount();

    while (1) {
        led_toggle(CONTROLS_HB_LED_PORT, CONTROLS_HB_LED_PIN);

        /**
         * @todo    implement fdcan1 rx driver here
         */

        vTaskDelayUntil(&xLastWakeTime, READ_CONTROLS_CAN_TASK_DELAY_TICKS);
    }
}