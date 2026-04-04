/**
 * @file ISR_test.c
 * @brief External interrupt test converted to FreeRTOS task.
 *        Verifies EXTI configuration for NEUTRAL_GEAR and IGN_OFF pins
 *        by blinking LSOM_HB at 0.5Hz as a heartbeat while interrupts are active.
 *        Interrupt behavior is handled in HAL_GPIO_EXTI_Callback.
 */

#include "FreeRTOS.h"
#include "Tasks.h"
#include "init.h"
#include "Status_LEDs.h"
#include "Switches.h"
#include "Horn.h"


#define GPIO_CALLBACK_TASK_DELAY_TICKS		pdMS_TO_TICKS(250)

/* Task control block and stack for the ISR test task */
static StaticTask_t ISR_TEST_TASK_TCB;
static StackType_t  ISR_TEST_TASK_Stack_Array[configMINIMAL_STACK_SIZE];

/**
 * @brief  FreeRTOS heartbeat task that blinks LSOM_HB at 0.5Hz
 *         to confirm the scheduler is running while EXTIs are active.
 * @param  argument  Unused task parameter.
 */
static void GPIO_Callback_Task(void *argument) {
    TickType_t xLastWakeTime = xTaskGetTickCount();

    while (1) {
        led_toggle(LSOM_HB_PORT, LSOM_HB_PIN);
        vTaskDelayUntil(&xLastWakeTime, GPIO_CALLBACK_TASK_DELAY_TICKS);
    }
}

int main(void) {
    HAL_Init();
    SystemClock_Config();
	led_GPIO_init();
    switch_GPIO_init();
    horn_GPIO_init();

    xTaskCreateStatic(
        GPIO_Callback_Task,
        "Testing Neutral Gear and Ign off gpio interrupts",
        configMINIMAL_STACK_SIZE,
        NULL,
        tskIDLE_PRIORITY + 1,
        ISR_TEST_TASK_Stack_Array,
        &ISR_TEST_TASK_TCB
    );

    vTaskStartScheduler();

    while (1) {}

    return 0;
}