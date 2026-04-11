#include "CarCAN.h"
#include "Horn.h"
#include "Status_LEDs.h"
#include "Switches.h"
#include "Tasks.h"
#include "init.h"

void Task_Send_Switch_States(void *argument) {
	TickType_t xLastWakeTime = xTaskGetTickCount();

	while (1) {
		uint32_t bitmap = switch_read_all_inputs();
		uint8_t tx_data_driver_status[CAN_DLC_DRIVER_INPUT_STATUS];

		CL_Pack_DriverStatus(bitmap, tx_data_driver_status);

		if (CarCAN_Send(CAN_ID_DRIVER_INPUT_STATUS, CAN_DLC_DRIVER_INPUT_STATUS, tx_data_driver_status, SEND_SWITCH_STATES_TASK_DELAY_TICKS) != CAN_OK) {
			led_toggle(AKSHAY_LED_PORT, AKSHAY_LED_PIN);
		}

		led_toggle(LSOM_HB_PORT, LSOM_HB_PIN);

		vTaskDelayUntil(&xLastWakeTime, SEND_SWITCH_STATES_TASK_DELAY_TICKS);
	}
}