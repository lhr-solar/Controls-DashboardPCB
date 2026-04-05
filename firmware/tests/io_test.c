/**
 * @file IO_test.c
 * @brief Basic GPIO input/output test converted to FreeRTOS task.
 *        Verifies switch input and LED output by toggling LSOM_HB at 1Hz
 *        and mirroring the IGN_OFF switch state to X_LED2.
 */

#include "FreeRTOS.h"
#include "Tasks.h"
#include "init.h"
#include "Switches.h"
#include "Horn.h"
#include "Status_LEDs.h"


#define IO_TASK_DELAY_TICKS		pdMS_TO_TICKS(250)


/* Task control block and stack for the IO test task */
static StaticTask_t IO_TEST_TASK_TCB;
static StackType_t  IO_TEST_TASK_Stack_Array[configMINIMAL_STACK_SIZE];

/**
 * @brief  FreeRTOS task that toggles LSOM_HB at 1Hz and mirrors
 *         the IGN_OFF switch state to X_LED2.
 * @param  argument  Unused task parameter.
 */
static void IOTest_Task(void *argument) {
    TickType_t xLastWakeTime = xTaskGetTickCount();

	while (1) {
		led_toggle(LSOM_HB_PORT, LSOM_HB_PIN);
		led_set(X_LED2_PORT, X_LED2_PIN, switch_get_state(SW_HAZARD));
		vTaskDelayUntil(&xLastWakeTime, IO_TASK_DELAY_TICKS);
	}
}

int main(void) {
    HAL_Init();
    SystemClock_Config();
	led_gpio_init();
    switch_init();
    horn_gpio_init();

    xTaskCreateStatic(
        IOTest_Task,
        "IO_test",
        configMINIMAL_STACK_SIZE,
        NULL,
        tskIDLE_PRIORITY + 1,
        IO_TEST_TASK_Stack_Array,
        &IO_TEST_TASK_TCB
    );

    vTaskStartScheduler();

    while (1) {}

    return 0;
}