#include "Tasks.h"
#include "init.h"
#include "Switches.h"
#include "Status_LEDs.h"
#include "Horn.h"
#include "CarCAN.h"

StackType_t readCarCAN_stackArray[READ_CAR_CAN_STACK_SIZE];
StackType_t readControlsCAN_stackArray[READ_CONTROLS_CAN_STACK_SIZE];
StackType_t send_switch_states_stackArray[SEND_SWITCH_STATES_STACK_SIZE];

StaticTask_t readCarCAN_tcb;
StaticTask_t readControlsCAN_tcb;
StaticTask_t send_switch_states_tcb;

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
        Task_Send_Switch_States,
        "Reading All Switches & Writing to CarCan",
        SEND_SWITCH_STATES_STACK_SIZE,
        NULL,
        SEND_SWITCH_STATES_PRIORITY,
        send_switch_states_stackArray,
        &send_switch_states_tcb
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

*/

    xTaskCreateStatic(
        ReadControlsCAN_task,
        "Read Controls CAN Task",
        READ_CONTROLS_CAN_STACK_SIZE,
        NULL,
        READ_CONTROLS_CAN_PRIORITY,
        readControlsCAN_stackArray,
        &readControlsCAN_tcb
    );

    vTaskDelete(NULL);
}

