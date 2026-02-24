#include "ControlsLeader_Tasks.h"
#include "init.h"

StackType_t READ_CAR_CAN_Stack_Array[READ_CAR_CAN_STACK_SIZE];
StackType_t READ_CONTROLS_CAN_Stack_Array[READ_CONTROLS_CAN_STACK_SIZE];
StackType_t READ_STEERING_ANGLE_CAN_Stack_Array[READ_STEERING_ANGLE_CAN_STACK_SIZE];
StackType_t POLLING_WRITE_CAN_Stack_Array[POLLING_WRITE_CAN_STACK_SIZE];

StaticTask_t READ_CAR_CAN_TCB;
StaticTask_t READ_CONTROLS_CAN_TCB;
StaticTask_t READ_STEERING_ANGLE_CAN_TCB;
StaticTask_t POLLING_WRITE_CAN_TCB;    

// SWITCH_STATES_EG = xEventGroupCreate();
// STATUS_LED_STATES_EG  = xEventGroupCreate();


void InitTasks(void *argument) {

    //Add BPS watchdog here

    //Task1.c
    xTaskCreateStatic(
        ReadCarCAN_task,
        "Read CarCAN Task",                    
        READ_CAR_CAN_STACK_SIZE,
        NULL,
        READ_CAR_CAN_PRIORITY,
        READ_CAR_CAN_Stack_Array,
        &READ_CAR_CAN_TCB
    );

    //Task2.c
    xTaskCreateStatic(
        Polling_WriteCAN_Task,
        "Polling Switches & Writing CAN Task", 
        POLLING_WRITE_CAN_STACK_SIZE,
        NULL,
        POLLING_WRITE_CAN_PRIORITY,
        POLLING_WRITE_CAN_Stack_Array,
        &POLLING_WRITE_CAN_TCB
    );

    //Task3.c
    xTaskCreateStatic(
        ReadSteeringAngCAN_task,
        "Read Steering Angle CAN Task",         
        READ_STEERING_ANGLE_CAN_STACK_SIZE,
        NULL,
        READ_STEERING_ANGLE_CAN_PRIORITY,
        READ_STEERING_ANGLE_CAN_Stack_Array,
        &READ_STEERING_ANGLE_CAN_TCB
    );

    //Task4.c
    xTaskCreateStatic(
        ReadControlsCAN_task,
        "Read Controls CAN Task",               
        READ_CONTROLS_CAN_STACK_SIZE,
        NULL,
        READ_CONTROLS_CAN_PRIORITY,
        READ_CONTROLS_CAN_Stack_Array,
        &READ_CONTROLS_CAN_TCB
    );

    vTaskDelete(NULL);

}