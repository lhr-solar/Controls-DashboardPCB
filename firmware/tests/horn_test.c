/**
 * @file Horn_test.c
 * @brief Horn output test converted to FreeRTOS task.
 *        Verifies horn output and LSOM_HB LED by toggling the LED at 1Hz
 *        and continuously mirroring the horn switch state to the horn output.
 */

#include "FreeRTOS.h"
#include "Tasks.h"
#include "init.h"
#include "Status_LEDs.h"
#include "Horn.h"
#include "Switches.h"

#define HORN_TASK_DELAY_TICKS		pdMS_TO_TICKS(250)

/* Task control block and stack for the horn test task */
static StaticTask_t HORN_TEST_TASK_TCB;
static StackType_t  HORN_TEST_TASK_Stack_Array[configMINIMAL_STACK_SIZE];

/**
 * @brief  FreeRTOS task that blinks LSOM_HB at 1Hz and mirrors
 *         the horn switch state to the horn output.
 * @param  argument  Unused task parameter.
 */
static void HornTest_Task(void *argument) {
	TickType_t xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();


	while (1) {
		led_toggle(LSOM_HB_PORT, LSOM_HB_PIN);
		horn_set(switch_get_state(SW_HORN));
		vTaskDelayUntil(&xLastWakeTime, HORN_TASK_DELAY_TICKS);
	}
}

int main(void) {
    HAL_Init();
    SystemClock_Config();
	led_GPIO_init();
    switch_GPIO_init();
    horn_GPIO_init();

    xTaskCreateStatic(
        HornTest_Task,
        "Horn_test",
        configMINIMAL_STACK_SIZE,
        NULL,
        tskIDLE_PRIORITY + 1,
        HORN_TEST_TASK_Stack_Array,
        &HORN_TEST_TASK_TCB
    );

    vTaskStartScheduler();

    while (1) {}

    return 0;
}