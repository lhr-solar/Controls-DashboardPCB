#include "Tasks.h"
#include "init.h"
#include "Status_LEDs.h"
#include "Horn.h"
#include "Switches.h"

void ReadControlsCAN_task(void *argument) {

    while(1) {
        led_set(CONTROLS_HB_LED_PORT, CONTROLS_HB_LED_PIN, GPIO_PIN_SET);
        vTaskDelay(pdMS_TO_TICKS(500));
        led_set(CONTROLS_HB_LED_PORT, CONTROLS_HB_LED_PIN, GPIO_PIN_RESET);
        vTaskDelay(pdMS_TO_TICKS(500));


		/**
		 * 
		 * @todo 	implement fdcan1 rx driver here
		 * 
		 */


    }
}