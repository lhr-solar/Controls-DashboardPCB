
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


#define SWITCH_BITMAP_TASK_DELAY_TICKS		pdMS_TO_TICKS(250)
#define NUM_STATUS_LEDS		8


/* Task control block and stack for the LED test task */
static StaticTask_t Switch_Bitmap_Test_Task_TCB;
static StackType_t  Switch_Bitmap_Test_Task_Stack_Array[configMINIMAL_STACK_SIZE];

typedef struct {
    GPIO_TypeDef *port;
    uint16_t      pin;
} LED_t;

static const LED_t statusLEDs[NUM_STATUS_LEDS] = {
    { X_LED2_PORT,        X_LED2_PIN        },    // bit 0
    { CAR_CAN_RX_LED_PORT, CAR_CAN_RX_LED_PIN },  // bit 1
    { CAR_CAN_TX_LED_PORT, CAR_CAN_TX_LED_PIN },  // bit 2
    { PH_CAN_RX_LED_PORT,  PH_CAN_RX_LED_PIN  },  // bit 3
    { PH_CAN_TX_LED_PORT,  PH_CAN_TX_LED_PIN  },  // bit 4
    { BPS_FAULT_LED_PORT,  BPS_FAULT_LED_PIN  },  // bit 5
    { CONTROLS_HB_LED_PORT,CONTROLS_HB_LED_PIN},  // bit 6
    { AKSHAY_LED_PORT,     AKSHAY_LED_PIN     },  // bit 7
};


static void displayStates_onLEDs() {
	uint32_t bits;

    portENTER_CRITICAL();
    bits = switch_bitmap_read();
    portEXIT_CRITICAL();

	for(int i = 0; i < NUM_STATUS_LEDS; i++) {
		GPIO_PinState state = (bits & GET_MASK(i)) ? GPIO_PIN_SET : GPIO_PIN_RESET;
    	led_set(statusLEDs[i].port, statusLEDs[i].pin, state);
	}
}


/**
 * @brief  Task to test stateBits bitamp
 * 
 * 		Toggles through all bits from 0 to 7. 
 * 		Success = LEDs on baord should flahs in a pattern (right to left)
 * 		Fail 	= Uh you aren't being flashed by LEDs :/
 * 
 * @param  argument  Unused task parameter.
 */
static void switch_bitmap_test(void *argument) {
	TickType_t xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();
	
	uint32_t i = 1;

    while (1) {
        led_toggle(LSOM_HB_PORT, LSOM_HB_PIN);

		displayStates_onLEDs();
		switch_bitmap_setAll(i);
		i += i;

		
        vTaskDelayUntil(&xLastWakeTime, SWITCH_BITMAP_TASK_DELAY_TICKS);
    }
}

int main(void) {
    HAL_Init();
    SystemClock_Config();
    led_GPIO_init();
    switch_GPIO_init();
    horn_GPIO_init();

    xTaskCreateStatic(
        switch_bitmap_test,
        "Toggle First 8 States",
        configMINIMAL_STACK_SIZE,
        NULL,
        tskIDLE_PRIORITY + 1,
        Switch_Bitmap_Test_Task_Stack_Array,
        &Switch_Bitmap_Test_Task_TCB
    );

    vTaskStartScheduler();

    while (1) {}
	
    return 0;
}