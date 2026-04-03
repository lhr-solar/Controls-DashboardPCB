#include "Tasks.h"
#include "init.h"
#include "Status_LEDs.h"
#include "Horn.h"
#include "Switches.h"

void ReadCarCAN_task(void *argument) {

    while(1) {
        led_set(LSOM_HB_PORT, LSOM_HB_PIN, GPIO_PIN_SET);
        vTaskDelay(pdMS_TO_TICKS(500));
        led_set(LSOM_HB_PORT, LSOM_HB_PIN, GPIO_PIN_RESET);
        vTaskDelay(pdMS_TO_TICKS(500));


		/**
		 * 
		 * @todo 	implement fdcan3 rx driver here
		 * 
		 */

    }
}