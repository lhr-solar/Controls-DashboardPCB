#include "Tasks.h"
#include "LightingCAN.h"
#include "init.h"
#include "Status_LEDs.h"
#include "Horn.h"
#include "Switches.h"

void ReadControlsCAN_task(void *argument) {
    TickType_t xLastWakeTime = xTaskGetTickCount();

	uint32_t bitmap = 0;
	unveiling_lighting_t lighting_mode = LIGHTS_OFF;


    while (1) {
		uint8_t tx_data_light_command[CAN_DLC_LIGHTING_COMMAND] = {0};
		bitmap = switch_read_all_inputs();

		if((bitmap >> SW_HAZARD) & 0x01){
			lighting_mode = LIGHTS_HOOK_EM;
		} else if((bitmap >> SW_CRUISE_ENABLE) & 0x01){
			lighting_mode = LIGHTS_FADE_IN_OUT;
		}

		CL_Pack_UnveilingLights(lighting_mode, tx_data_light_command);
		if(LightingCAN_Send(CAN_ID_LIGHTING_COMMAND, CAN_DLC_LIGHTING_COMMAND, tx_data_light_command, CONTROLS_CAN_TASK_DELAY_TICKS) != CAN_OK){
			led_toggle(AKSHAY_LED_PORT, AKSHAY_LED_PIN);
		}

        
		led_toggle(CONTROLS_HB_LED_PORT, CONTROLS_HB_LED_PIN);
        vTaskDelayUntil(&xLastWakeTime, CONTROLS_CAN_TASK_DELAY_TICKS);
    }
}