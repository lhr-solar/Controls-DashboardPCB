#pragma once

#include <stdint.h>
#include "stm32xx_hal.h"
#include "pinDefs.h"
#include "CAN_FD.h"
#include "CarCAN_can_msgs.h"
#include "CarCAN.h"

extern FDCAN_HandleTypeDef *steering_hfdcan;;

/**
 * @brief 				 Initiliaize FDCAN1 - SteeringCAN Peripheral
 * @return can_status_t  success = CAN_OK, fail = anything else get to debugging lil man
 */
can_status_t SteeringCAN_Init(void);