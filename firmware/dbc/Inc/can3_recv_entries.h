#include "CarCAN.h"

CAN_RECV_ENTRY(CAN_ID_BPS_STATUS, 1, true)          //circular queue = true, fifo depth = 1
CAN_RECV_ENTRY(CAN_ID_VCU_STATUS, 1, true)          //circular queue = true, fifo depth = 1
CAN_RECV_ENTRY(CAN_ID_BRAKE_PRESSURE_1, 1, true)    //circular queue = true, fifo depth = 1
CAN_RECV_ENTRY(CAN_ID_BRAKE_PRESSURE_2, 1, true)    //circular queue = true, fifo depth = 1