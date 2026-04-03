/**
 * @file GET_FLASHED_test.c
 * @brief LED flash test converted to FreeRTOS task.
 *        Verifies GPIO init and LED output by flashing all LEDs
 *        repeatedly from a dedicated FreeRTOS task.
 */

#include "FreeRTOS.h"
#include "Tasks.h"
#include "init.h"
#include "Status_LEDs.h"

/* Task control block and stack for the flash task */
static StaticTask_t FLASH_TASK_TCB;
static StackType_t  FLASH_TASK_Stack_Array[configMINIMAL_STACK_SIZE];

/**
 * @brief  FreeRTOS task that flashes all LEDs repeatedly.
 *         Replaces the bare-metal while(1) loop from the original test
 * 
 * @param  argument  Unused task parameter
 */
static void FlashLEDs_Task(void *argument) {
    while (1) {
        flash_them();
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

int main(void) {
    HAL_Init();
    SystemClock_Config();
    GPIO_Init();

    xTaskCreateStatic(
        FlashLEDs_Task,
        "GET_FLASHED",
        configMINIMAL_STACK_SIZE,
        NULL,
        tskIDLE_PRIORITY + 1,
        FLASH_TASK_Stack_Array,
        &FLASH_TASK_TCB
    );

    vTaskStartScheduler();

    while (1) {}

    return 0;
}