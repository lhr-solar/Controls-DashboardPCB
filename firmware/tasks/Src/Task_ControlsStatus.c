#include "Tasks.h"
#include "CarCAN.h"
#include "CarCAN_can_msgs.h"
#include "init.h"
#include "Status_LEDs.h"
#include "Horn.h"
#include "Switches.h"

static TimerHandle_t bps_watchdog_timer;
static StaticTimer_t bps_timer_buffer;

static void vBPSWatchdogCallback(TimerHandle_t timer)
{
    // No BPS messages received within timeout
	// Only resets with car power cycle
    set_high_noon_state(BPS_FAULT, ON);

}

void ReadCarCAN_task(void *argument) {
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

    while (1) {
        
		uint8_t data[CAN_DLC_VCU_STATUS] = {0};

		if(CarCAN_Receive(CAN_ID_VCU_STATUS, data, READ_CARCAN_TASK_DELAY_TICKS) == CAN_OK) {
			xTimerReset(bps_watchdog_timer, 0);
		}

		uint8_t regen_status = (data[2] >> 1) & 0x1;
		set_high_noon_state(VCU_REGEN_STATUS, regen_status ? ON : OFF);

		uint8_t brake_status = (data[4] >> 1) & 0x1;
		set_high_noon_state(VCU_BRAKE_STATUS, brake_status ? ON : OFF);

        vTaskDelayUntil(&xLastWakeTime, READ_CARCAN_TASK_DELAY_TICKS);
    }
}