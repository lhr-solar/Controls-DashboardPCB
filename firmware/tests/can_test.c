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
#include "Debugging.h"


#define CAN_TEST_TASK_DELAY_TICKS		pdMS_TO_TICKS(2000)

static StaticTask_t CAN_Test_Task_TCB;
static StackType_t  CAN_Test_Task_Stack[configMINIMAL_STACK_SIZE];

static can_status_t CL_SendDriverStatus_Test(uint8_t* tx_data) {
	return CarCAN_Send(CAN_ID_DRIVER_INPUT_STATUS, CAN_DLC_DRIVER_INPUT_STATUS, tx_data, CAN_TEST_TASK_DELAY_TICKS);
}

//static can_status_t CL_ReadDriverStatus_Test(uint8_t rx_data[8]) {
//    return CarCAN_Receive(CAN_ID_DRIVER_INPUT_STATUS, rx_data, CAN_TEST_TASK_DELAY_TICKS);
//}

void CAN_Test(void *argument) {
	initPrintf();

	TickType_t xLastWakeTime = xTaskGetTickCount();
	
	uint32_t payload = 0;

	uint8_t	tx_data[8];
	//uint8_t rx_data[8];

	while(1) {
		printf("bruh\n\r");
			
		tx_data[0] = payload;
		tx_data[1] = 0;
		tx_data[2] = 0;
		tx_data[3] = 0;

		CL_SendDriverStatus_Test(tx_data);
		payload += 1;

		//if(CL_ReadDriverStatus_Test(rx_data) != CAN_OK) {
		//	printf("data: %d\n\r", rx_data[0]);
		//} else led_toggle(AKSHAY_LED_PORT, AKSHAY_LED_PIN);
		
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
        CAN_Test,
        "Can Send & Read Test Task",
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