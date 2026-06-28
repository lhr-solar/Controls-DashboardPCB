#pragma once

#include <stdint.h>
#include "stm32xx_hal.h"
#include "pinDefs.h"
#include "CAN_FD.h"
#include "CarCAN_can_msgs.h"

extern FDCAN_HandleTypeDef *CarCAN;

/**
 * @brief 				 Initiliaize FDCAN3 - CarCAN Peripheral
 * @return can_status_t  success = CAN_OK, fail = anything else get to debugging bro
 */
can_status_t CarCAN_Init(void);

/**
 * @brief  Wrapper function for sending payload over CarCAN
 * @param  id   			CAN ID of message (ID macro)
 * @param  payloadSize_dlc	size of CAN payload (DLC macro)
 * @param  data  			payload to send over CAN
 * @param  delay_ticks  	timeout time (in ticks)
 * @return can_status_t  	success = CAN_OK, fail = anything else get to debugging bro
 */
can_status_t CarCAN_Send(uint32_t id, uint32_t payloadSize_dlc, uint8_t* data, TickType_t delay_ticks);

can_status_t CarCAN_Receive(uint32_t id, uint8_t data[], TickType_t delay_ticks);

/**
 * @brief  Packs bitmap into dbc format
 * @param  bitmap   switch_states bitmap, holds all switch states
 * @param  tx_data  formatted data to be sent on CAN
 * @return NONE
 */
void CL_Pack_DriverStatus(uint16_t bitmap, uint8_t* tx_data);