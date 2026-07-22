#include "CarCAN.h"
#include "Horn.h"
#include "Status_LEDs.h"
#include "Switches.h"
#include "Tasks.h"
#include "init.h"
#include "printf.h"
#include "overrides.h"
#include "SteeringCAN.h"

static void print_changed_switches(uint32_t previous_bitmap, uint32_t new_bitmap) {

	// see which bits have changed
	const uint32_t changed_bits = previous_bitmap ^ new_bitmap;
	if (changed_bits == 0U) {
		return;
	}

	printf("Switches changed: ");
	for (size_t i = 0; i < SW_COUNT; i++) {
		if (((changed_bits >> i) & 0x1U) != 0U) {
			printf("%s changed from state: %u to : %u", switch_names[i], (unsigned int)((previous_bitmap >> i) & 0x1U), (unsigned int)((new_bitmap >> i) & 0x1U));
		}
	}
	printf("\r\n");
}

void Task_Send_Switch_States(void *argument) {
	TickType_t xLastWakeTime = xTaskGetTickCount();

	uint32_t reset_ign = 0;
	uint32_t reset_gear = 0;
	uint32_t bitmap = 0;
	uint32_t previous_bitmap = 0;
	uint32_t steering_angle_reset = 0;

	while (1) {
		previous_bitmap = bitmap;
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

		if((((bitmap >> SW_REGEN_ENABLE) & 0x1) == 1) && (((bitmap >> SW_CRUISE_ENABLE) & 0x1) == 1)) {
			steering_angle_reset += 1;
		} else {
			steering_angle_reset = 0;
		}

		if(steering_angle_reset >= 10 && ((bitmap >> SW_REV) & 0x1) != 1) {
			if(SteeringCAN_ResetAngle(SEND_SWITCH_STATES_TASK_DELAY_TICKS) != CAN_OK) {
				printf("SteeringCAN_ResetAngle failed to send\r\n");
			}
			steering_angle_reset = 0;
		} else if(steering_angle_reset >= 10 && ((bitmap >> SW_REV) & 0x1) == 1) {
			if(SteeringCAN_ResetCalibration(SEND_SWITCH_STATES_TASK_DELAY_TICKS) != CAN_OK) {
				printf("SteeringCAN_ResetCalibration failed to send\r\n");
			}
			steering_angle_reset = 0;
		}

		if((bitmap >> SW_IGN_MTR) & 0x1) bitmap |= (0x1 << SW_IGN_ARR);

		uint8_t tx_data_driver_status[CAN_DLC_DRIVER_INPUT_STATUS];

		CL_Pack_DriverStatus(bitmap, tx_data_driver_status);

		if (CarCAN_Send(CAN_ID_DRIVER_INPUT_STATUS, CAN_DLC_DRIVER_INPUT_STATUS,
						tx_data_driver_status,
						SEND_SWITCH_STATES_TASK_DELAY_TICKS) != CAN_OK) {
			led_toggle(AKSHAY_LED_PORT, AKSHAY_LED_PIN);
		}

		Send_Overrides();
		
		horn_set(switch_get_state(SW_HORN));
		led_toggle(LSOM_HB_PORT, LSOM_HB_PIN);

		print_changed_switches(previous_bitmap, bitmap);


		vTaskDelayUntil(&xLastWakeTime, SEND_SWITCH_STATES_TASK_DELAY_TICKS);
	}
}