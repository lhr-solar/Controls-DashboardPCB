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
        
		//uint8_t data[8] = {0};

		//if(CarCAN_Receive(CAN_ID_DRIVER_INPUT_STATUS, data, READ_CARCAN_TASK_DELAY_TICKS) == CAN_EMPTY) {
		//	led_toggle(X_LED2_PORT, X_LED2_PIN);
		//} 

        vTaskDelayUntil(&xLastWakeTime, READ_CARCAN_TASK_DELAY_TICKS);
    }
}