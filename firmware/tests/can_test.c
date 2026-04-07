/* Copyright (c) 2018-2026 UT Longhorn Racing Solar */
/** CarCAN_test.c
 * Description: Integration task for CarCAN + ElconCAN charging control.
 *              Charges via Elcon when BPS grants permission (BPS_Charge_OK=1).
 *              Stops immediately and faults on BPS denial or any Elcon fault flag.
 *              Propagates Elcon faults back to CarCAN as BPS_Charge_OK=0.
 * Hardware: Elcon connected on LV/HV. BPS broadcasting on CarCAN (FDCAN3).
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "init.h"
#include "Status_LEDs.h"
#include "Switches.h"
#include "Horn.h"
#include "CarCAN.h"


#define CAN_TEST_TASK_DELAY_TICKS		pdMS_TO_TICKS(250)

static StaticTask_t CAN_Test_Task_TCB;
static StackType_t  CAN_Test_Task_Stack[configMINIMAL_STACK_SIZE];

static can_status_t CL_SendDriverStatus(uint32_t driver_status_payload) {

	uint8_t tx_data[8] = {0};

	tx_data[0] = driver_status_payload & ~(0xFFFFFFF0);
	tx_data[1] = driver_status_payload & ~(0xFFFFFF0F);
	tx_data[2] = driver_status_payload & ~(0xFFFFF0FF);
	tx_data[3] = driver_status_payload & ~(0xFFFF0FFF);
	tx_data[4] = driver_status_payload & ~(0xFFF0FFFF);

	return CarCAN_Send(CAN_ID_DRIVER_INPUT_STATUS, tx_data, CAN_TEST_TASK_DELAY_TICKS);
}

void CAN_Send_Test(void *argument) {

	TickType_t xLastWakeTime = xTaskGetTickCount();
	
	uint32_t payload = 0x00054321;

	while(1) {
		if (CL_SendDriverStatus(payload) != CAN_OK) {
			led_toggle(AKSHAY_LED_PORT, AKSHAY_LED_PIN);
		}

		payload -= 5;
		
		led_toggle(LSOM_HB_PORT, LSOM_HB_PIN);
		vTaskDelayUntil(&xLastWakeTime, CAN_TEST_TASK_DELAY_TICKS);
	}
}

int main(void) {
    HAL_Init();
    SystemClock_Config();
	CarCAN_Init();
	led_gpio_init();
    switch_init();
    horn_gpio_init();

    xTaskCreateStatic(
        CAN_Send_Test,
        "Can Send Test Task",
        configMINIMAL_STACK_SIZE,
        NULL,
        tskIDLE_PRIORITY + 1,
        CAN_Test_Task_Stack,
        &CAN_Test_Task_TCB
	);

    vTaskStartScheduler();

    while (1) {}

    return 0;
}