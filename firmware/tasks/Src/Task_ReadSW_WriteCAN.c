#include "Tasks.h"
#include "init.h"
#include "Status_LEDs.h"
#include "Horn.h"
#include "Switches.h"

#define READ_WRITE_CARCAN_TASK_DELAY_TICKS 	pdMS_TO_TICKS(250)

void Read_Switches_WriteCAN_Task(void *argument) {
    TickType_t xLastWakeTime = xTaskGetTickCount();

    while (1) {
        led_toggle(X_LED2_PORT, X_LED2_PIN);

        portENTER_CRITICAL();
        switch_bitmap_setBit(SW_IGN_ARR, SWITCH_ON);
        portEXIT_CRITICAL();

        /**
         * @todo    implement read sw drivers and update switch_states[]
         *          then send that as payload over fdcan1 (ControlsCAN)
         *          and fdcan3 (CarCAN)
         */

        vTaskDelayUntil(&xLastWakeTime, READ_WRITE_CARCAN_TASK_DELAY_TICKS);
    }
}