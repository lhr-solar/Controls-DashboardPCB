/**
 * @file LED_test.c
 * @brief Basic LED output test converted to FreeRTOS task.
 *        Verifies LED GPIO init and output by blinking LSOM_HB at 1Hz.
 */

#include "FreeRTOS.h"
#include "Tasks.h"
#include "init.h"
#include "Status_LEDs.h"

/* Task control block and stack for the LED test task */
static StaticTask_t LED_TEST_TASK_TCB;
static StackType_t  LED_TEST_TASK_Stack_Array[configMINIMAL_STACK_SIZE];

/**
 * @brief  FreeRTOS task that blinks LSOM_HB at 1Hz to verify
 *         LED GPIO initialization and output.
 * @param  argument  Unused task parameter.
 */
static void LEDTest_Task(void *argument) {
    while (1) {
        led_set(LSOM_HB_PORT, LSOM_HB_PIN, GPIO_PIN_SET);
        vTaskDelay(pdMS_TO_TICKS(500));
        led_set(LSOM_HB_PORT, LSOM_HB_PIN, GPIO_PIN_RESET);
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

int main(void) {
    HAL_Init();
    SystemClock_Config();
    GPIO_Init();

    xTaskCreateStatic(
        LEDTest_Task,
        "LED_test",
        configMINIMAL_STACK_SIZE,
        NULL,
        tskIDLE_PRIORITY + 1,
        LED_TEST_TASK_Stack_Array,
        &LED_TEST_TASK_TCB
    );

    vTaskStartScheduler();

    while (1) {}

    return 0;
}