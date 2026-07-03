#pragma once

#include <stdint.h>
#include "stm32xx_hal.h"
#include "pinDefs.h"
#include "CAN_FD.h"
#include "CarCAN_can_msgs.h"
#include "CarCAN.h"
#include "SteeringCAN_can_msgs.h"

extern FDCAN_HandleTypeDef *steering_hfdcan;;

/**
 * @brief 				 Initiliaize FDCAN1 - SteeringCAN Peripheral
 * @return can_status_t  success = CAN_OK, fail = anything else get to debugging lil man
 */
can_status_t SteeringCAN_Init(void);

/**
 * @brief  Wrapper function for sending payload over steeringCAN
 * @param  id   			CAN ID of message (ID macro)
 * @param  payloadSize_dlc	size of CAN payload (DLC macro)
 * @param  data  			payload to send over CAN
 * @param  delay_ticks  	timeout time (in ticks)
 * @return can_status_t  	success = CAN_OK, fail = CAN_ERR
 */
can_status_t SteeringCAN_Send(uint32_t id, uint32_t payloadSize_dlc, uint8_t* data, TickType_t delay_ticks);