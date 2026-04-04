#include "FreeRTOS.h"
#include "Tasks.h"
#include "init.h"
#include "Switches.h"
#include "Status_LEDs.h"
#include "Horn.h"

StaticTask_t INIT_TASK_TCB;
StackType_t INIT_TASK_Stack_Array[INIT_TASK_STACK_SIZE];

int main(void) {
    HAL_Init();
	SystemClock_Config();

    xTaskCreateStatic(
        InitTasks,
        "Init All Tasks",
        INIT_TASK_STACK_SIZE,
        NULL,
        INIT_TASK_PRIORITY,
        INIT_TASK_Stack_Array,
        &INIT_TASK_TCB
    );
    vTaskStartScheduler();

    while(1) {}

    return 0;
}