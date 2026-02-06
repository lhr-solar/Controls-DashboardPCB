#include "ControlsLeader_Tasks.h"

StackType_t READ_CAR_CAN_Stack_Array[READ_CAR_CAN_STACK_SIZE];
StackType_t READ_CONTROLS_CAN_Stack_Array[READ_CONTROLS_CAN_STACK_SIZE];
StackType_t READ_STEERING_ANGLE_CAN_Stack_Array[READ_STEERING_ANGLE_CAN_STACK_SIZE];
StackType_t UPDATE_SWITCH_STATES_Stack_Array[UPDATE_SWITCH_STATES_STACK_SIZE];

StaticTask_t READ_CAR_CAN_TCB;
StaticTask_t READ_CONTROLS_CAN_TCB;
StaticTask_t READ_STEERING_ANGLE_CAN_TCB;
StaticTask_t UPDATE_SWITCH_STATES_TCB;     

void InitTasks(void *argument) {

    //Add BPS watchdog here

    xTaskCreateStatic(
        ReadCarCAN_task,
        "Read CarCAN Task",                    // for now its only LSOM HB blinky
        READ_CAR_CAN_STACK_SIZE,
        NULL,
        READ_CAR_CAN_PRIORITY,
        READ_CAR_CAN_Stack_Array,
        &READ_CAR_CAN_TCB
    );

    xTaskCreateStatic(
        UpdateSwitchStatesCAN_task,
        "Polling Switches & Writing CAN Task", //for now its only a Status LED blinky
        UPDATE_SWITCH_STATES_STACK_SIZE,
        NULL,
        UPDATE_SWITCH_STATES_PRIORITY,
        UPDATE_SWITCH_STATES_Stack_Array,
        &UPDATE_SWITCH_STATES_TCB
    );

    vTaskDelete(NULL);

}