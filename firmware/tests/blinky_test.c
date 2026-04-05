/**
 * @file LED_test.c
 * @brief Basic LED output test converted to FreeRTOS task.
 *        Verifies LED GPIO init and output by blinking LSOM_HB at 1Hz.
 */

#include "FreeRTOS.h"
#include "Tasks.h"
#include "init.h"
#include "Switches.h"
#include "Horn.h"
#include "Status_LEDs.h"

#define BLINKY_TASK_DELAY_TICKS		pdMS_TO_TICKS(250)

/* Task control block and stack for the LED test task */
static StaticTask_t LED_TEST_TASK_TCB;
static StackType_t  LED_TEST_TASK_Stack_Array[configMINIMAL_STACK_SIZE];

/**
 * @brief  FreeRTOS task that blinks LSOM_HB at 1Hz to verify
 *         LED GPIO initialization and output.
 * @param  argument  Unused task parameter.
 */
static void LEDTest_Task(void *argument) {
    TickType_t xLastWakeTime = xTaskGetTickCount();

    while (1) {
        led_toggle(LSOM_HB_PORT, LSOM_HB_PIN);
        vTaskDelayUntil(&xLastWakeTime, BLINKY_TASK_DELAY_TICKS);
    }
}

int main(void) {
    HAL_Init();
    SystemClock_Config();
    led_gpio_init();
    switch_init();
    horn_gpio_init();

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