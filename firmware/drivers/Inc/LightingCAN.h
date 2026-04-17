#pragma once

#include <stdint.h>
#include "stm32xx_hal.h"
#include "pinDefs.h"
#include "CAN_FD.h"
#include "LightingCAN_can_msgs.h"


typedef enum {
	LIGHTS_OFF = 0,
	LIGHTS_RGB = 2,
	LIGHTS_HOOK_EM = 4,
	LIGHTS_FADE_IN_OUT = 8
} unveiling_lighting_t;


/**
 * @brief 				 Initiliaize FDCAN1 - CarCAN Peripheral
 * @return can_status_t  success = CAN_OK, fail = anything else get to debugging bro
 */
can_status_t LightingCAN_Init(void);

/**
 * @brief  Wrapper function for sending payload over LightingCAN
 * @param  id   			CAN ID of message (ID macro)
 * @param  payloadSize_dlc	size of CAN payload (DLC macro)
 * @param  data  			payload to send over CAN
 * @param  delay_ticks  	timeout time (in ticks)
 * @return can_status_t  	success = CAN_OK, fail = anything else get to debugging bro
 */
can_status_t LightingCAN_Send(uint32_t id, uint32_t payloadSize_dlc, uint8_t* data, TickType_t delay_ticks);


void CL_Pack_UnveilingLights(unveiling_lighting_t lighting_mode, uint8_t* tx_data);