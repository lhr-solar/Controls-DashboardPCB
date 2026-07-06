#pragma once

#include <stdint.h>
#include "stm32xx_hal.h"
#include "pinDefs.h"
#include "CAN_FD.h"
#include "LightingCAN_can_msgs.h"

extern FDCAN_HandleTypeDef *LightingCAN;


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

can_status_t LightingCAN_SendLightingCommand(lighting_command_t command, TickType_t delay_ticks);