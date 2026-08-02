/**
 * @file reset_steering_angle_test.c
 * @brief Test for resetting steering angle.
 */

#include "FreeRTOS.h"
#include "Tasks.h"
#include "init.h"
#include "Switches.h"
#include "Horn.h"
#include "Status_LEDs.h"
#include "Debugging.h"
#include "SteeringCAN.h"

#define STEERING_RESET_DELAY_TICKS		pdMS_TO_TICKS(250)

/* Task control block and stack for the LED test task */
static StaticTask_t STEERING_RESET_TASK_TCB;
static StackType_t  STEERING_RESET_TASK_Stack_Array[configMINIMAL_STACK_SIZE];

/**
 * @brief  FreeRTOS task that blinks LSOM_HB at 1Hz to verify
 *         LED GPIO initialization and output.
 * @param  argument  Unused task parameter.
 */
static void STEERING_RESET_Task(void *argument) {
    
    TickType_t xLastWakeTime = xTaskGetTickCount();
    vTaskDelay(pdMS_TO_TICKS(5000)); // Wait for 1 second before starting the test

    while (1) {
        led_toggle(LSOM_HB_PORT, LSOM_HB_PIN);
        SteeringCAN_ResetCalibration(STEERING_RESET_DELAY_TICKS);
        vTaskDelayUntil(&xLastWakeTime, STEERING_RESET_DELAY_TICKS);
        SteeringCAN_ResetAngle(STEERING_RESET_DELAY_TICKS);
        vTaskDelayUntil(&xLastWakeTime, STEERING_RESET_DELAY_TICKS);
    }
}

int main(void) {

    HAL_Init();
	led_gpio_init();
    switch_init();
	CarCAN_Init();
	SteeringCAN_Init();
    initPrintf();

    xTaskCreateStatic(
        STEERING_RESET_Task,
        "Reset Steering Angle Task",
        configMINIMAL_STACK_SIZE,
        NULL,
        tskIDLE_PRIORITY + 1,
        STEERING_RESET_TASK_Stack_Array,
        &STEERING_RESET_TASK_TCB
    );

    vTaskStartScheduler();

    while (1) {}
	
    return 0;
}