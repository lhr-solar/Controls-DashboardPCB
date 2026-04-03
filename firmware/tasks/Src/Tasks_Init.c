#include "Tasks.h"
#include "init.h"

StackType_t readCarCAN_stackArray[READ_CAR_CAN_STACK_SIZE];
StackType_t readControlsCAN_stackArray[READ_CONTROLS_CAN_STACK_SIZE];
StackType_t readSteeringAngleCAN_stackArray[READ_STEERING_ANGLE_CAN_STACK_SIZE];
StackType_t pollingWriteCAN_stackArray[POLLING_WRITE_CAN_STACK_SIZE];

StaticTask_t readCarCAN_tcb;
StaticTask_t readControlsCAN_tcb;
StaticTask_t readSteeringAngleCAN_tcb;
StaticTask_t pollingWriteCAN_tcb;


// SWITCH_STATES_EG = xEventGroupCreate();
// STATUS_LED_STATES_EG  = xEventGroupCreate();


void InitTasks(void *argument) {

    /**
	 * 
	 * @todo     Add BPS fault watchdog here
	 * 
	*/

    //Task 1
    xTaskCreateStatic(
        ReadCarCAN_task,
        "Read CarCAN Task",
        READ_CAR_CAN_STACK_SIZE,
        NULL,
        READ_CAR_CAN_PRIORITY,
        readCarCAN_stackArray,
        &readCarCAN_tcb
    );

    //Task 2
    xTaskCreateStatic(
        Polling_WriteCAN_Task,
        "Polling Switches & Writing CAN Task",
        POLLING_WRITE_CAN_STACK_SIZE,
        NULL,
        POLLING_WRITE_CAN_PRIORITY,
        pollingWriteCAN_stackArray,
        &pollingWriteCAN_tcb
    );

    //Task 3
    xTaskCreateStatic(
        ReadSteeringAngCAN_task,
        "Read Steering Angle CAN Task",
        READ_STEERING_ANGLE_CAN_STACK_SIZE,
        NULL,
        READ_STEERING_ANGLE_CAN_PRIORITY,
        readSteeringAngleCAN_stackArray,
        &readSteeringAngleCAN_tcb
    );

    //Task 4
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

