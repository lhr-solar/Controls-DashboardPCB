#include "Tasks.h"
#include "LightingCAN.h"
#include "init.h"
#include "Status_LEDs.h"
#include "Horn.h"
#include "Switches.h"

void ReadControlsCAN_task(void *argument) {
    TickType_t xLastWakeTime = xTaskGetTickCount();

	
	lighting_command_t lighting_command = {0};
	uint8_t tx_payload[CAN_DLC_LIGHTING_COMMAND] = {0};

    while (1) {
		

		///**
		// * 
		// * Hazard -> create a while loop that runs while button is on, and have lights
		// * flash periodically?
		// * 
		// * Brake -> needs to be read from CarCAN and update lighting status
		// * 
		// * BPS_Strobe -> needs to be read from CarCAN and udpate lighting status
		// * 
		// */


        // --- HAZARD LOGIC WITH INTERRUPTIBLE DELAY ---
        if ((switch_bitmap_read() >> SW_HAZARD) & 0x1) {

            while ((switch_bitmap_read() >> SW_HAZARD) & 0x1) {

                lighting_command.Lighting_Blink_Sync ^= 0x1;

                uint32_t notified = 0;
                BaseType_t result = xTaskNotifyWait(0, 0, &notified, HAZARD_PERIOD_TICKS);

                if (result == pdTRUE) {
                    // hazard turned off → break immediately
                    break;
                }
            }
        }


		lighting_command.Lighting_Set_Left_Indicator = (switch_bitmap_read() >> SW_LEFT_BLINKER) & 0x1;
		lighting_command.Lighting_Set_Right_Indicator = (switch_bitmap_read() >> SW_RIGHT_BLINKER) & 0x1;
		lighting_command.Lighting_Set_BPS_Strobe = 0;
		lighting_command.Lighting_Set_Headlights = 0;
		lighting_command.Lighting_Set_Custom_Mode = 0;
		lighting_command.Lighting_Set_Brake = 0;


		if(LightingCAN_Send(CAN_ID_LIGHTING_COMMAND, CAN_DLC_LIGHTING_COMMAND, tx_payload, CONTROLS_CAN_TASK_DELAY_TICKS) != CAN_OK){
			led_toggle(BPS_FAULT_LED_PORT, BPS_FAULT_LED_PIN);
		}

		led_toggle(CONTROLS_HB_LED_PORT, CONTROLS_HB_LED_PIN);
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(500));
    }
}