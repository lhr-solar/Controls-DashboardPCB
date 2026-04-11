#include "Tasks.h"
#include "init.h"
#include "Status_LEDs.h"
#include "Horn.h"
#include "Switches.h"
#include "CarCAN.h"


void Read_Switches_WriteCAN_Task(void *argument) {
    TickType_t xLastWakeTime = xTaskGetTickCount();

    while (1) {
		uint32_t bitmap = switch_read_all_inputs();

		if(CL_SendDriverStatus(bitmap, READ_WRITE_CARCAN_TASK_DELAY_TICKS) != CAN_OK) {
			led_toggle(AKSHAY_LED_PORT, AKSHAY_LED_PIN);
		}

        vTaskDelayUntil(&xLastWakeTime, READ_WRITE_CARCAN_TASK_DELAY_TICKS);
    }
}