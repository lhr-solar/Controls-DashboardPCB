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
    GPIO_Init();
    switch_EXTI_Init(NEUTRAL_GEAR_PORT, NEUTRAL_GEAR_PIN, 5);
    switch_EXTI_Init(IGN_OFF_PORT, IGN_OFF_PIN, 4);

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