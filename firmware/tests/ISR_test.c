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

/* Task control block and stack for the ISR test task */
static StaticTask_t ISR_TEST_TASK_TCB;
static StackType_t  ISR_TEST_TASK_Stack_Array[configMINIMAL_STACK_SIZE];

/**
 * @brief  FreeRTOS heartbeat task that blinks LSOM_HB at 0.5Hz
 *         to confirm the scheduler is running while EXTIs are active.
 * @param  argument  Unused task parameter.
 */
static void ISRTest_Task(void *argument) {
    while (1) {
        led_set(LSOM_HB_PORT, LSOM_HB_PIN, GPIO_PIN_SET);
        vTaskDelay(pdMS_TO_TICKS(1000));
        led_set(LSOM_HB_PORT, LSOM_HB_PIN, GPIO_PIN_RESET);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

int main(void) {
    HAL_Init();
    SystemClock_Config();
    GPIO_Init();
    switch_EXTI_Init(NEUTRAL_GEAR_PORT, NEUTRAL_GEAR_PIN, 5);
    switch_EXTI_Init(IGN_OFF_PORT, IGN_OFF_PIN, 4);

    xTaskCreateStatic(
        ISRTest_Task,
        "ISR_test",
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