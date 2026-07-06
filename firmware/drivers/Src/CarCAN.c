#include "CarCAN.h"
#include "Horn.h"
#include "Status_LEDs.h"
#include "Switches.h"
#include "init.h"
#include "CarCAN_can_msgs.h"

/* ================= CarCAN (fdcan3) ================= */
FDCAN_HandleTypeDef *CarCAN = NULL;
static FDCAN_RxHeaderTypeDef carCAN_rx_header;

static can_status_t CarCAN_Recover(void) {
	if ((CarCAN == NULL) || (CarCAN->Instance == NULL)) {
		return CAN_ERR;
	}

	if (can_fd_start(CarCAN) != CAN_OK) {
		return CAN_ERR;
	}

	return CAN_OK;
}

can_status_t CarCAN_Init(void) {
	CarCAN = hfdcan3;
	if (CarCAN == NULL) {
		return CAN_ERR;
	}

	CarCAN->Instance = FDCAN3;
	CarCAN->Init.ClockDivider = FDCAN_CLOCK_DIV1;
	CarCAN->Init.FrameFormat = FDCAN_FRAME_CLASSIC;
	CarCAN->Init.Mode = FDCAN_MODE_NORMAL;
	CarCAN->Init.AutoRetransmission = ENABLE;
	CarCAN->Init.TransmitPause = DISABLE;
	CarCAN->Init.ProtocolException = DISABLE;
	CarCAN->Init.NominalPrescaler = 20;
	CarCAN->Init.NominalSyncJumpWidth = 1;
	CarCAN->Init.NominalTimeSeg1 = 13;
	CarCAN->Init.NominalTimeSeg2 = 2;
	CarCAN->Init.DataPrescaler = 1;
	CarCAN->Init.DataSyncJumpWidth = 1;
	CarCAN->Init.DataTimeSeg1 = 1;
	CarCAN->Init.DataTimeSeg2 = 1;
	CarCAN->Init.StdFiltersNbr = 1;
	CarCAN->Init.ExtFiltersNbr = 0;
	CarCAN->Init.TxFifoQueueMode = FDCAN_TX_FIFO_OPERATION;

	FDCAN_FilterTypeDef sFilterConfig = {0};
	sFilterConfig.IdType = FDCAN_STANDARD_ID;
	sFilterConfig.FilterIndex = 0;
	sFilterConfig.FilterType = FDCAN_FILTER_MASK;
	sFilterConfig.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;

	// no filter yet
	sFilterConfig.FilterID1 = 0x00000000;
	sFilterConfig.FilterID2 = 0x00000000;

	if (can_fd_init(CarCAN, &sFilterConfig) != CAN_OK) {
		return CAN_ERR;
	}

	if (can_fd_start(CarCAN) != CAN_OK) {
		return CAN_ERR;
	}

	return CAN_OK;
}

can_status_t CarCAN_Send(uint32_t id, uint32_t payloadSize_dlc, uint8_t* data, TickType_t delay_ticks) {
	FDCAN_TxHeaderTypeDef carCAN_tx_header = {
		.Identifier = id,
		.IdType = FDCAN_STANDARD_ID,
		.TxFrameType = FDCAN_DATA_FRAME,
		.DataLength = payloadSize_dlc,
		.ErrorStateIndicator = FDCAN_ESI_ACTIVE,
		.BitRateSwitch = FDCAN_BRS_OFF,
		.FDFormat = FDCAN_CLASSIC_CAN,
		.TxEventFifoControl = FDCAN_NO_TX_EVENTS,
		.MessageMarker = 0,
	};

	if ((CarCAN == NULL) || (data == NULL)) return CAN_ERR;
	if (can_fd_send(CarCAN, &carCAN_tx_header, data, delay_ticks) == CAN_ERR) {
		if (CarCAN_Recover() != CAN_OK) {
			led_set(CAR_CAN_TX_LED_PORT, CAR_CAN_TX_LED_PIN, LED_OFF);
			return CAN_ERR;
		}

		if (can_fd_send(CarCAN, &carCAN_tx_header, data, delay_ticks) == CAN_ERR) {
			led_set(CAR_CAN_TX_LED_PORT, CAR_CAN_TX_LED_PIN, LED_OFF);
			return CAN_ERR;
		}
	}

	led_toggle(CAR_CAN_TX_LED_PORT, CAR_CAN_TX_LED_PIN);
	return CAN_OK;
}

can_status_t CarCAN_Receive(uint32_t id, uint8_t data[], TickType_t delay_ticks) {
	if ((CarCAN == NULL) || (data == NULL)) {
		return CAN_ERR;
	}

	can_status_t rx_status = can_fd_recv(CarCAN, id, &carCAN_rx_header, data, delay_ticks);
	if(rx_status != CAN_OK) {
		if (CarCAN_Recover() != CAN_OK) {
			led_set(CAR_CAN_RX_LED_PORT, CAR_CAN_RX_LED_PIN, LED_ON);
			return rx_status;
		}

		rx_status = can_fd_recv(CarCAN, id, &carCAN_rx_header, data, delay_ticks);
		if (rx_status != CAN_OK) {
			led_set(CAR_CAN_RX_LED_PORT, CAR_CAN_RX_LED_PIN, LED_ON);
			return rx_status;
		}
	}

	led_toggle(CAR_CAN_RX_LED_PORT, CAR_CAN_RX_LED_PIN);
	return CAN_OK;
}

can_status_t CarCAN_Recv_BPS_Status(bps_status_t *out, TickType_t delay) {
    if (out == NULL) return CAN_EMPTY;

    FDCAN_RxHeaderTypeDef header = {0};
    uint8_t bps_status_rx_data[CAN_DLC_BPS_STATUS] = {0};

    can_status_t result =
        can_fd_recv(CarCAN, CAN_ID_BPS_STATUS, &header, bps_status_rx_data, delay);

    if (result == CAN_OK) {
        out->BPS_Fault = bps_status_rx_data[0];
        out->BPS_Regen_OK = (bps_status_rx_data[1] >> 0) & 1;
        out->BPS_Charge_OK = (bps_status_rx_data[1] >> 1) & 1;
        out->HV_Plus_Contactor_State = (bps_status_rx_data[1] >> 2) & 1;
        out->HV_Minus_Contactor_State = (bps_status_rx_data[1] >> 3) & 1;
        out->Array_Contactor_State = (bps_status_rx_data[1] >> 4) & 1;
        out->Array_Precharge_Contactor_State = (bps_status_rx_data[1] >> 5) & 1;
        out->Main_Battery_Voltage = (uint32_t)(bps_status_rx_data[4] | 
                                    ((uint32_t)bps_status_rx_data[5] << 8) |
                                    ((uint32_t)bps_status_rx_data[6] << 16) |
                                    ((uint32_t)bps_status_rx_data[7] << 24));
        out->Main_Battery_Avg_Temperature = (int16_t)((uint16_t)bps_status_rx_data[2] | ((uint16_t)bps_status_rx_data[3] << 8));
    }
    return result;
}

can_status_t CarCAN_Recv_Brake_Pressure1(brake_pressure_1_t *out, TickType_t delay) {
    if (out == NULL) return CAN_EMPTY;

    FDCAN_RxHeaderTypeDef header = {0};
    uint8_t brake_pressure1_rx_data[CAN_DLC_BRAKE_PRESSURE_1] = {0};

    can_status_t result =
        can_fd_recv(CarCAN, CAN_ID_BRAKE_PRESSURE_1, &header, brake_pressure1_rx_data, delay);
        
    if (result == CAN_OK) {
        out->Brake_Pressure = (uint16_t)(brake_pressure1_rx_data[0] | ((uint16_t)brake_pressure1_rx_data[1] << 8));
        out->Brake_Pressure_ADC = (uint16_t)(brake_pressure1_rx_data[2] | ((uint16_t)brake_pressure1_rx_data[3] << 8));
        out->FrameID_Pedals = brake_pressure1_rx_data[4];
    }

    return result;
}

can_status_t CarCAN_Recv_Brake_Pressure2(brake_pressure_2_t *out, TickType_t delay) {
    if (out == NULL) return CAN_EMPTY;

    FDCAN_RxHeaderTypeDef header = {0};
    uint8_t brake_pressure2_rx_data[CAN_DLC_BRAKE_PRESSURE_2] = {0};

    can_status_t result =
        can_fd_recv(CarCAN, CAN_ID_BRAKE_PRESSURE_2, &header, brake_pressure2_rx_data, delay);
        
    if (result == CAN_OK) {
        out->Brake_Pressure = (uint16_t)(brake_pressure2_rx_data[0] | ((uint16_t)brake_pressure2_rx_data[1] << 8));
        out->Brake_Pressure_ADC = (uint16_t)(brake_pressure2_rx_data[2] | ((uint16_t)brake_pressure2_rx_data[3] << 8));
        out->FrameID_Pedals = brake_pressure2_rx_data[4];
    }

    return result;
}

can_status_t CarCAN_Recv_VCU_Status(vcu_status_t *out, TickType_t delay) {
	if (out == NULL) return CAN_EMPTY;

	FDCAN_RxHeaderTypeDef header = {0};
	uint8_t vcu_status_rx_data[CAN_DLC_VCU_STATUS] = {0};

	can_status_t result =
		can_fd_recv(CarCAN, CAN_ID_VCU_STATUS, &header, vcu_status_rx_data, delay);
		
	if (result == CAN_OK) {
		// bits 0 - 3: VCU_FSM_State
		out->VCU_FSM_State = vcu_status_rx_data[0] & 0x0F;
		// bit 4: Motor Ready
		out->Motor_Ready = (vcu_status_rx_data[0] >> 4) & 0x01;
		// bit 5: Motor_Precharge_Contactor_State
		out->Motor_Precharge_Contactor_State = (vcu_status_rx_data[0] >> 5) & 0x01;
		// bit 6: Motor_Contactor_State
		out->Motor_Contactor_State = (vcu_status_rx_data[0] >> 6) & 0x01;
		// bit 7: VCU_Driver_Input_Watchdog
		out->VCU_Driver_Input_Watchdog = (vcu_status_rx_data[0] >> 7) & 0x01;
		// bits 8 - 15: VCU_Pedals_Watchdog, VCU_BPS_Watchdog, VCU_Steering_Angle_Watchdog, VCU_BPS_FAULT_DETECTED, VCU_CONTROLS_FAULT_DETECTED, VCU_MTR_FAULT_DETECTED, VCU_PEDALS_FAULT_DETECTED, VCU_STEERING_FAULT_DETECTED
		out->VCU_Pedals_Watchdog = vcu_status_rx_data[1] & 0x01;
		out->VCU_BPS_Watchdog = (vcu_status_rx_data[1] >> 1) & 0x01;
		out->VCU_Steering_Angle_Watchdog = (vcu_status_rx_data[1] >> 2) & 0x01;
		out->VCU_BPS_FAULT_DETECTED = (vcu_status_rx_data[1] >> 3) & 0x01;
		out->VCU_CONTROLS_FAULT_DETECTED = (vcu_status_rx_data[1] >> 4) & 0x01;
		out->VCU_MTR_FAULT_DETECTED = (vcu_status_rx_data[1] >> 5) & 0x01;
		out->VCU_PEDALS_FAULT_DETECTED = (vcu_status_rx_data[1] >> 6) & 0x01;
		out->VCU_STEERING_FAULT_DETECTED = (vcu_status_rx_data[1] >> 7) & 0x01;
		// bits 16 - 23: VCU_MotorCommandSource, VCU_Regen_Active, VCU_Regen_OK, VCU_MTR_PCHG_TIMEOUT, VCU_MTR_PCHG_CONT_TIMEOUT, VCU_MTR_PCHG_CONT_MISMATCH, VCU_MTR_CONT_MISMATCH, VCU_MTR_CONT_TIMEOUT
		out->VCU_MotorCommandSource = vcu_status_rx_data[2] & 0x01;
		out->VCU_Regen_Active = (vcu_status_rx_data[2] >> 1) & 0x01;
		out->VCU_Regen_OK = (vcu_status_rx_data[2] >> 2) & 0x01;
		out->VCU_MTR_PCHG_TIMEOUT = (vcu_status_rx_data[2] >> 3) & 0x01;
		out->VCU_MTR_PCHG_CONT_TIMEOUT = (vcu_status_rx_data[2] >> 4) & 0x01;
		out->VCU_MTR_PCHG_CONT_MISMATCH = (vcu_status_rx_data[2] >> 5) & 0x01;
		out->VCU_MTR_CONT_MISMATCH = (vcu_status_rx_data[2] >> 6) & 0x01;
		out->VCU_MTR_CONT_TIMEOUT = (vcu_status_rx_data[2] >> 7) & 0x01;
		// bits 24 - 31: VCU_PCHG_OV, VCU_PCHG_UV, VCU_MTR_OV, VCU_MTR_UV, VCU_OTHER_FAULT, VCU_MTR_DIR_CHANGE_LOCKOUT, VCU_TIPPING_WARNING, VCU_WARN_REGEN_NOT_ALLOW
		out->VCU_PCHG_OV = vcu_status_rx_data[3] & 0x01;
		out->VCU_PCHG_UV = (vcu_status_rx_data[3] >> 1) & 0x01;
		out->VCU_MTR_OV = (vcu_status_rx_data[3] >> 2) & 0x01;
		out->VCU_MTR_UV = (vcu_status_rx_data[3] >> 3) & 0x01;
		out->VCU_OTHER_FAULT = (vcu_status_rx_data[3] >> 4) & 0x01;
		out->VCU_MTR_DIR_CHANGE_LOCKOUT = (vcu_status_rx_data[3] >> 5) & 0x01;
		out->VCU_TIPPING_WARNING = (vcu_status_rx_data[3] >> 6) & 0x01;
		out->VCU_WARN_REGEN_NOT_ALLOW = (vcu_status_rx_data[3] >> 7) & 0x01;
		// bits 32 - 39: VCU_WARN_REGEN_NOT_EN, VCU_FSM_INP_BRAKE, VCU_FSM_INP_PCHG_OK, VCU_FSM_INP_CRUISE_REQ, VCU_FSM_INP_REGEN_REQ, VCU_FSM_INP_REGEN_ENABLE, VCU_FSM_INP_REGEN_RDY, VCU_FSM_INP_FORWARD
		out->VCU_WARN_REGEN_NOT_EN = vcu_status_rx_data[4] & 0x01;
		out->VCU_FSM_INP_BRAKE = (vcu_status_rx_data[4] >> 1) & 0x01;
		out->VCU_FSM_INP_PCHG_OK = (vcu_status_rx_data[4] >> 2) & 0x01;
		out->VCU_FSM_INP_CRUISE_REQ = (vcu_status_rx_data[4] >> 3) & 0x01;
		out->VCU_FSM_INP_REGEN_REQ = (vcu_status_rx_data[4] >> 4) & 0x01;
		out->VCU_FSM_INP_REGEN_ENABLE = (vcu_status_rx_data	[4] >> 5) & 0x01;
		out->VCU_FSM_INP_REGEN_RDY = (vcu_status_rx_data[4] >> 6) & 0x01;
		out->VCU_FSM_INP_FORWARD = (vcu_status_rx_data[4] >> 7) & 0x01;
		// bits 40 - 47: VCU_FSM_INP_NEUTRAL, VCU_FSM_INP_REVERSE
		out->VCU_FSM_INP_NEUTRAL = vcu_status_rx_data[5] & 0x01;
		out->VCU_FSM_INP_REVERSE = (vcu_status_rx_data[5] >> 1) & 0x01;	
	}

	return result;
}

void CL_Pack_DriverStatus(uint16_t bitmap, uint8_t* tx_data) {
    if(tx_data == NULL) return;

    // byte 0 - bits 7-0
    tx_data[0]  = 0;
    tx_data[0] |= ((bitmap >> SW_IGN_ARR)       & 0x1) << 0; //change the numbers after << to change bit positions on payload
    tx_data[0] |= ((bitmap >> SW_IGN_MTR)       & 0x1) << 1; // << to change bit positions on payload
    tx_data[0] |= ((bitmap >> SW_IGN_OFF)       & 0x1) << 2;
    tx_data[0] |= ((bitmap >> SW_CRUISE_ENABLE) & 0x1) << 3;
    tx_data[0] |= ((bitmap >> SW_CRUISE_SET)    & 0x1) << 4;
    tx_data[0] |= ((bitmap >> SW_FWD)           & 0x1) << 5;
    tx_data[0] |= ((bitmap >> SW_NEUTRAL_GEAR)  & 0x1) << 6;
    tx_data[0] |= ((bitmap >> SW_REV)           & 0x1) << 7;

    // byte 1 - bits 15-8
    tx_data[1]  = 0;
    tx_data[1] |= ((bitmap >> SW_HAZARD)        & 0x1) << 0;
    tx_data[1] |= ((bitmap >> SW_HORN)          & 0x1) << 1;
    tx_data[1] |= ((bitmap >> SW_LEFT_BLINKER)  & 0x1) << 2;
    tx_data[1] |= ((bitmap >> SW_RIGHT_BLINKER) & 0x1) << 3;
    tx_data[1] |= ((bitmap >> SW_PTT)           & 0x1) << 4;
    tx_data[1] |= ((bitmap >> SW_REGEN_ACTIVE)  & 0x1) << 5;
    tx_data[1] |= ((bitmap >> SW_REGEN_ENABLE)  & 0x1) << 6;
}
