#include "Tasks.h"
#include "init.h"
#include "Status_LEDs.h"
#include "Horn.h"
#include "Switches.h"


EventGroupHandle_t faultStateBits;
static uint8_t switch_states[SW_COUNT] = {0};

void Polling_WriteCAN_Task(void *argument) {

    while(1) {
        led_set(X_LED2_PORT, X_LED2_PIN, GPIO_PIN_SET);
        vTaskDelay(pdMS_TO_TICKS(500));
        led_set(X_LED2_PORT, X_LED2_PIN, GPIO_PIN_RESET);
        vTaskDelay(pdMS_TO_TICKS(500));

		UNUSED(switch_states);

		portENTER_CRITICAL();
		switch_bitmap_setBit(SW_IGN_ARR, SWITCH_ON);
		portEXIT_CRITICAL();
		
		/**
		 * 
		 * @todo 	implement read sw drivers and update switch_states[]
		 * 			then send that as payload over fdcan1 (ControlsCAN)	
		 * 			and fdcan3 (CarCAN)
		 * 
		 */
    }
}