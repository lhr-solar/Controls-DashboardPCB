#include "Tasks.h"
#include "init.h"
#include "Switches.h"
#include "Status_LEDs.h"
#include "Horn.h"
#include "CarCAN.h"

StackType_t readCarCAN_stackArray[READ_CAR_CAN_STACK_SIZE];
StackType_t readControlsCAN_stackArray[READ_CONTROLS_CAN_STACK_SIZE];
StackType_t pollingWriteCAN_stackArray[POLLING_WRITE_CAN_STACK_SIZE];
StackType_t heartbeat_stackArray[HEARTBEAT_STACK_SIZE];

StaticTask_t readCarCAN_tcb;
StaticTask_t readControlsCAN_tcb;
StaticTask_t pollingWriteCAN_tcb;
StaticTask_t heartbeat_tcb;

void InitTasks(void *argument) {

    /**
	 * 
	 * @todo     Add BPS fault watchdog here
	 * 
	*/

	CarCAN_Init();
	led_gpio_init();
    switch_init();
    horn_gpio_init();


	xTaskCreateStatic(
        HeartBeat,
        "LSOM Heartbeat",
        HEARTBEAT_STACK_SIZE,
        NULL,
        HEARTBEAT_PROIRITY,
        pollingWriteCAN_stackArray,
        &heartbeat_tcb
    );
	
	xTaskCreateStatic(
        Read_Switches_WriteCAN_Task,
        "Reading All Switches & Writing CAN Task",
        POLLING_WRITE_CAN_STACK_SIZE,
        NULL,
        POLLING_WRITE_CAN_PRIORITY,
        heartbeat_stackArray,
        &pollingWriteCAN_tcb
    );

/*
    xTaskCreateStatic(
        ReadCarCAN_task,
        "Read CarCAN Task",
        READ_CAR_CAN_STACK_SIZE,
        NULL,
        READ_CAR_CAN_PRIORITY,
        readCarCAN_stackArray,
        &readCarCAN_tcb
    );

    xTaskCreateStatic(
        ReadControlsCAN_task,
        "Read Controls CAN Task",
        READ_CONTROLS_CAN_STACK_SIZE,
        NULL,
        READ_CONTROLS_CAN_PRIORITY,
        readControlsCAN_stackArray,
        &readControlsCAN_tcb
    );
*/

    vTaskDelete(NULL);
}

void HeartBeat(void *argument) {
    TickType_t xLastWakeTime = xTaskGetTickCount();

    while (1) {
		
		led_toggle(LSOM_HB_PORT, LSOM_HB_PIN);

        vTaskDelayUntil(&xLastWakeTime, HEARTBEAT_DELAY_TICKS);
    }
}

