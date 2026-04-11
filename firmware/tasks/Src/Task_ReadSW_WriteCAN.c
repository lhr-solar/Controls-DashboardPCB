#include "Tasks.h"
#include "init.h"
#include "Status_LEDs.h"
#include "Horn.h"
#include "Switches.h"
#include "CarCAN.h"


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

void Read_Switches_WriteCAN_Task(void *argument) {
    TickType_t xLastWakeTime = xTaskGetTickCount();

    while (1) {
		uint32_t bitmap = switch_read_all_inputs();

		if(CL_SendDriverStatus(bitmap, READ_WRITE_CARCAN_TASK_DELAY_TICKS) != CAN_OK) {
			led_toggle(AKSHAY_LED_PORT, AKSHAY_LED_PIN);
		}

		displayStates_onLEDs();

        vTaskDelayUntil(&xLastWakeTime, READ_WRITE_CARCAN_TASK_DELAY_TICKS);
    }
}