#include "FreeRTOS.h"
#include "ControlsLeader_Tasks.h"
#include "init.h"

StaticTask_t INIT_TASK_TCB;
StackType_t INIT_TASK_Stack_Array[INIT_TASK_STACK_SIZE];

int main(int argc, char const *argv[]) {
    HAL_Init();
    GPIO_Init();
    
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

    while(1){
        
    }
    
    return 0;
}
