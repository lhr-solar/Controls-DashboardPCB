#include "Tasks.h"
#include "init.h"
#include "Switches.h"
#include "Status_LEDs.h"
#include "Horn.h"

StackType_t readCarCAN_stackArray[READ_CAR_CAN_STACK_SIZE];
StackType_t readControlsCAN_stackArray[READ_CONTROLS_CAN_STACK_SIZE];
StackType_t pollingWriteCAN_stackArray[POLLING_WRITE_CAN_STACK_SIZE];

StaticTask_t readCarCAN_tcb;
StaticTask_t readControlsCAN_tcb;
StaticTask_t pollingWriteCAN_tcb;


void InitTasks(void *argument) {

    /**
	 * 
	 * @todo     Add BPS fault watchdog here
	 * 
	*/

    led_gpio_init();
    switch_init();
    horn_gpio_init();
	

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
        Read_Switches_WriteCAN_Task,
        "Reading All Switches & Writing CAN Task",
        POLLING_WRITE_CAN_STACK_SIZE,
        NULL,
        POLLING_WRITE_CAN_PRIORITY,
        pollingWriteCAN_stackArray,
        &pollingWriteCAN_tcb
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

    vTaskDelete(NULL);
}

