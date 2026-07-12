#include "Tasks.h"
#include "LightingCAN.h"
#include "init.h"
#include "Status_LEDs.h"
#include "Horn.h"
#include "Switches.h"
#include "CarCAN_can_msgs.h"
#include "CarCAN.h"
#include "LightingCAN_can_msgs.h"
#include "printf.h"

#define BRAKE_PRESSURE_THRESH_PSI 120
#define BRAKE_PRESSURE_THRESH_HYSTERESIS_PSI 10

static TimerHandle_t bps_watchdog_timer;
static StaticTimer_t bps_timer_buffer;

static void vBPSWatchdogCallback(TimerHandle_t timer)
{
    // No BPS messages received within timeout
	// Only resets with car power cycle
    set_high_noon_state(BPS_FAULT, ON);

}

static void print_updated_lighting_command(lighting_command_t old_command, lighting_command_t new_command){
    int first = 1;
    int printed = 0;

    if(old_command.Lighting_Set_Headlights != new_command.Lighting_Set_Headlights){
        if(!printed){
            printf("Lighting Command Update: {");
            printed = 1;
        }
        printf("%sHeadlights = %s", first ? "" : ", ", new_command.Lighting_Set_Headlights ? "ON" : "OFF");
        first = 0;
    }

    if(old_command.Lighting_Set_Left_Indicator != new_command.Lighting_Set_Left_Indicator){
        if(!printed){
            printf("Lighting Command Update: {");
            printed = 1;
        }
        printf("%sLEFT_INDICATOR = %s", first ? "" : ", ", new_command.Lighting_Set_Left_Indicator ? "ON" : "OFF");
        first = 0;
    }

    if(old_command.Lighting_Set_Right_Indicator != new_command.Lighting_Set_Right_Indicator){
        if(!printed){
            printf("Lighting Command Update: {");
            printed = 1;
        }
        printf("%sRIGHT_INDICATOR = %s", first ? "" : ", ", new_command.Lighting_Set_Right_Indicator ? "ON" : "OFF");
        first = 0;
    }

    if(old_command.Lighting_Blink_Sync != new_command.Lighting_Blink_Sync){
        if(!printed){
            printf("Lighting Command Update: {");
            printed = 1;
        }
        printf("%sBLINK_SYNC = %s", first ? "" : ", ", new_command.Lighting_Blink_Sync ? "ON" : "OFF");
        first = 0;
    }

    if(old_command.Lighting_Set_Brake != new_command.Lighting_Set_Brake){
        if(!printed){
            printf("Lighting Command Update: {");
            printed = 1;
        }
        printf("%sBrake = %s", first ? "" : ", ", new_command.Lighting_Set_Brake ? "ON" : "OFF");
        first = 0;
    }

    if(old_command.Lighting_Set_BPS_Strobe != new_command.Lighting_Set_BPS_Strobe){
        if(!printed){
            printf("Lighting Command Update: {");
            printed = 1;
        }
        printf("%sBPS_STROBE = %s", first ? "" : ", ", new_command.Lighting_Set_BPS_Strobe ? "ON" : "OFF");
        first = 0;
    }

    if(printed){
        printf("}\r\n");
    }
}

void Task_Send_Lighting_Commands(void *argument) {
    TickType_t xLastWakeTime = xTaskGetTickCount();

	bps_watchdog_timer = xTimerCreateStatic(
    	"BPS Watchdog",
    	pdMS_TO_TICKS(BPS_WATCHDOG_TIMEOUT_MS),
    	pdFALSE,
    	NULL,
    	vBPSWatchdogCallback,
    	&bps_timer_buffer
	);

	xTimerStart(bps_watchdog_timer, 0);

	lighting_command_t lighting_command = {0};
    lighting_command_t old_lighting_command = {0};
    bps_status_t bps_status = {0};
    brake_pressure_2_t brake_pressure_2 = {0};

    while (1) {


		uint8_t data[CAN_DLC_VCU_STATUS] = {0};
		uint8_t regen_status = 0;
		uint8_t brake_status = 0;
		uint8_t motor_ready_status = 0;

		if(CarCAN_Receive(CAN_ID_VCU_STATUS, data, READ_CARCAN_TASK_DELAY_TICKS) == CAN_OK) {
			regen_status = (data[2] >> 1) & 0x1;
			brake_status = (data[4] >> 1) & 0x1;
			motor_ready_status = (data[0] >> 4) & 0x1;


			// only need to turn on headlights if the car is in a driveable state
        	if(motor_ready_status == 1){
				lighting_command.Lighting_Set_Headlights = LIGHTING_COMMAND_LIGHTING_SET_HEADLIGHTS_ON;
			}
        	else if(motor_ready_status == 0){
            	lighting_command.Lighting_Set_Headlights = LIGHTING_COMMAND_LIGHTING_SET_HEADLIGHTS_OFF;
        	}

			if(regen_status == 1 || brake_status == 1){
				lighting_command.Lighting_Set_Brake = LIGHTING_COMMAND_LIGHTING_SET_BRAKE_ON;
}
			else {
				lighting_command.Lighting_Set_Brake = LIGHTING_COMMAND_LIGHTING_SET_BRAKE_OFF;
			}

			xTimerReset(bps_watchdog_timer, 0);
		}

        // save a copy so we can print out the difference
        old_lighting_command = lighting_command;

        // in general, if we fail to read a CAN message then we can default to the previous values
        // everything is off by default at the start

        // no need for custom messages in production firmware
        lighting_command.Lighting_Set_Custom_Mode = 0;

        CarCAN_Recv_BPS_Status(&bps_status, 0);
        CarCAN_Recv_Brake_Pressure2(&brake_pressure_2, 0);

        // BPS has faulted, turn on bps strobe and hazards
        // BPS fault is latching so don't need to have logic to clear strobe and hazards in a bps fault
        if(bps_status.BPS_Fault != BPS_STATUS_BPS_FAULT_OK){
            lighting_command.Lighting_Set_BPS_Strobe = LIGHTING_COMMAND_LIGHTING_SET_BPS_STROBE_ON;
            lighting_command.Lighting_Blink_Sync = 1;

            // turn on hazards
            lighting_command.Lighting_Set_Left_Indicator = LIGHTING_COMMAND_LIGHTING_SET_LEFT_INDICATOR_ON;
            lighting_command.Lighting_Set_Right_Indicator = LIGHTING_COMMAND_LIGHTING_SET_RIGHT_INDICATOR_ON;
        }

        bool hazards_enabled = ((switch_bitmap_read() >> SW_HAZARD) & 0x1);
        if(hazards_enabled){
            lighting_command.Lighting_Set_Left_Indicator = LIGHTING_COMMAND_LIGHTING_SET_LEFT_INDICATOR_ON;
            lighting_command.Lighting_Set_Right_Indicator = LIGHTING_COMMAND_LIGHTING_SET_RIGHT_INDICATOR_ON;
        }
        // even if the hazard switch was not pressed, if we're in a BPS fault state we want to keep the hazards on
        else if(bps_status.BPS_Fault == BPS_STATUS_BPS_FAULT_OK) {
            // if the hazards are not set and bps is in a good state, left and right indicators can be set based on switch input
            lighting_command.Lighting_Set_Left_Indicator = (switch_bitmap_read() >> SW_LEFT_BLINKER) & 0x1 ? LIGHTING_COMMAND_LIGHTING_SET_LEFT_INDICATOR_ON : LIGHTING_COMMAND_LIGHTING_SET_LEFT_INDICATOR_OFF;
            lighting_command.Lighting_Set_Right_Indicator = (switch_bitmap_read() >> SW_RIGHT_BLINKER) & 0x1 ? LIGHTING_COMMAND_LIGHTING_SET_RIGHT_INDICATOR_ON : LIGHTING_COMMAND_LIGHTING_SET_RIGHT_INDICATOR_OFF;
        }

        // send lighting command on carcan and light can
		if(LightingCAN_SendLightingCommand(lighting_command, CONTROLS_CAN_TASK_DELAY_TICKS) != CAN_OK){
			led_toggle(CONTROLS_HB_LED_PORT, CONTROLS_HB_LED_PIN);
		}

        print_updated_lighting_command(old_lighting_command, lighting_command);


        // leds are cleared on the lighting board after 500ms of no lighting command, so we send at higher rate than that
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(300));
    }
}