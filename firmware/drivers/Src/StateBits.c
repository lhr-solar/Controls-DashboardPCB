#include "StateBits.h"

// Event group handle to store state bits
EventGroupHandle_t stateBits;
// Static buffer to store the event handle
StaticEventGroup_t stateBitsBuffer;

// Switch state array index map:
// Index  | Switch
// -------|----------------
//   0    | Ign_ARR
//   1    | Ign_MTR
//   2    | Cruise_Enable
//   3    | Cruise_Set
//   4    | FWD_SW
//   5    | Neutral_Gear
//   6    | Rev_SW
//   7    | Hazard
//   8    | Left_Blinker
//   9    | Right_Blinker
//  10    | Horn
//  11    | PTT
//  12    | Regen_Enable
//  13    | Regen_Active
//  14    | BPS Fault


CL_status_t stateBits_init(void) {
	stateBits = xEventGroupCreateStatic(&stateBitsBuffer);
	if(stateBits == NULL) return 0;
	return CL_OK;
}


void stateBit_set(state_bit_t bit, switch_state_t state) {
	if(bit >= SW_COUNT) return;

	uint16_t mask = GET_MASK(bit);

	if(state == ON) xEventGroupSetBits(stateBits, mask);
	else xEventGroupClearBits(stateBits, mask);

	taskYIELD();
}


void stateBit_set_fromISR(state_bit_t bit, switch_state_t state) {
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    if(bit >= SW_COUNT) return;
	uint16_t mask = GET_MASK(bit);


	if(state == ON) {
		xEventGroupSetBitsFromISR(
			stateBits,
			mask,
			&xHigherPriorityTaskWoken
		);
	} else {
		xEventGroupClearBitsFromISR(
			stateBits,
			mask
		);
	}

	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
	
}