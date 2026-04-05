/**
 * @file GET_FLASHED_test.c
 * @brief LED flash test converted to FreeRTOS task.
 *        Verifies GPIO init and LED output by flashing all LEDs
 *        repeatedly from a dedicated FreeRTOS task.
 */

#include "FreeRTOS.h"
#include "Tasks.h"
#include "init.h"
#include "Switches.h"
#include "Horn.h"
#include "Status_LEDs.h"


#define GET_FLASHED_TASK_DELAY_TICKS		pdMS_TO_TICKS(250)


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
    TickType_t xLastWakeTime = xTaskGetTickCount();

    while (1) {
        flash_them();
        vTaskDelayUntil(&xLastWakeTime, GET_FLASHED_TASK_DELAY_TICKS);
    }
}

int main(void) {
    HAL_Init();
    SystemClock_Config();
	led_gpio_init();
    switch_init();
    horn_gpio_init();

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