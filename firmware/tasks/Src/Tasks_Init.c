#include "Tasks.h"
#include "init.h"
#include "Switches.h"
#include "Status_LEDs.h"
#include "Horn.h"
#include "CarCAN.h"
#include "LightingCAN.h"
#include "SteeringCAN.h"
#include "Debugging.h"

StackType_t readCarCAN_stackArray[READ_CAR_CAN_STACK_SIZE];
StackType_t readControlsCAN_stackArray[READ_CONTROLS_CAN_STACK_SIZE];
StackType_t send_switch_states_stackArray[SEND_SWITCH_STATES_STACK_SIZE];
StackType_t send_lighting_commands_stackArray[SEND_LIGHTING_COMMANDS_STACK_SIZE];

StaticTask_t readCarCAN_tcb;
StaticTask_t readControlsCAN_tcb;
StaticTask_t send_switch_states_tcb;
StaticTask_t send_lighting_commands_tcb;

TaskHandle_t ReadControlsCAN_TaskHandle;

void InitTasks(void *argument) {

	led_gpio_init();
    switch_init();
    horn_gpio_init();
	CarCAN_Init();
	SteeringCAN_Init();
	LightingCAN_Init();
    initPrintf();
	
	set_high_noon_state(BPS_FAULT, OFF);
	
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
        Task_Send_Switch_States,
        "Reading All Switches & Writing to CarCan",
        SEND_SWITCH_STATES_STACK_SIZE,
        NULL,
        SEND_SWITCH_STATES_PRIORITY,
        send_switch_states_stackArray,
        &send_switch_states_tcb
    );

    xTaskCreateStatic(
        Task_Send_Lighting_Commands,
        "Sending Lighting Commands",
        SEND_LIGHTING_COMMANDS_STACK_SIZE,
        NULL,
        SEND_LIGHTING_COMMANDS_PRIORITY,
        send_lighting_commands_stackArray,
        &send_lighting_commands_tcb
    );

    vTaskDelete(NULL);
}

