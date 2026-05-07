#include "CarCAN.h"
#include "Horn.h"
#include "Status_LEDs.h"
#include "Switches.h"
#include "Tasks.h"
#include "init.h"


void Task_Send_Switch_States(void *argument) {
	TickType_t xLastWakeTime = xTaskGetTickCount();

	uint32_t reset_ign = 0;
	uint32_t reset_gear = 0;
	uint32_t bitmap = 0;

	while (1) {
		bitmap = switch_read_all_inputs();

		if ((((bitmap >> SW_HAZARD) & 0x1)) && (ReadControlsCAN_TaskHandle != NULL)) {
    		xTaskNotifyGive(ReadControlsCAN_TaskHandle);
		}

		if((bitmap >> SW_NEUTRAL_GEAR) & 0x01){
			reset_gear = 1;
		}
		if((bitmap >> SW_IGN_OFF) & 0x01){
			reset_ign = 1;
		}

		if (reset_ign == 0 || (((bitmap >> SW_IGN_OFF) & 0x01) == 0 && ((bitmap >> SW_IGN_MTR) & 0x01) == 0 && ((bitmap >> SW_IGN_ARR) & 0x01) == 0)) {
			bitmap |= (0x1 << SW_IGN_OFF);
			bitmap &= ~(0x1 << SW_IGN_MTR);
			bitmap &= ~(0x1 << SW_IGN_ARR);
		}

		if(reset_gear == 0 || (((bitmap >> SW_NEUTRAL_GEAR) & 0x01) == 0 && ((bitmap >> SW_FWD) & 0x01) == 0 && ((bitmap >> SW_REV) & 0x01) == 0)){
			bitmap &= ~(0x1 << SW_FWD);
			bitmap &= ~(0x1 << SW_REV);
			bitmap |= (0x1 << SW_NEUTRAL_GEAR);
		}

		if((bitmap >> SW_IGN_MTR)&0x1) bitmap |= (0x1 << SW_IGN_ARR);

		uint8_t tx_data_driver_status[CAN_DLC_DRIVER_INPUT_STATUS];

		CL_Pack_DriverStatus(bitmap, tx_data_driver_status);

		if (CarCAN_Send(CAN_ID_DRIVER_INPUT_STATUS, CAN_DLC_DRIVER_INPUT_STATUS,
						tx_data_driver_status,
						SEND_SWITCH_STATES_TASK_DELAY_TICKS) != CAN_OK) {
			led_toggle(AKSHAY_LED_PORT, AKSHAY_LED_PIN);
		}

		led_toggle(LSOM_HB_PORT, LSOM_HB_PIN);

		vTaskDelayUntil(&xLastWakeTime, SEND_SWITCH_STATES_TASK_DELAY_TICKS);
	}
}