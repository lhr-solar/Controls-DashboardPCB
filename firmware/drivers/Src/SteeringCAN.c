#include "SteeringCAN.h"
#include "LightingCAN.h"
#include "Status_LEDs.h"

static uint32_t fdcan_dlc_from_bytes(uint32_t len) {
    switch (len) {
        case 0:  return FDCAN_DLC_BYTES_0;
        case 1:  return FDCAN_DLC_BYTES_1;
        case 2:  return FDCAN_DLC_BYTES_2;
        case 3:  return FDCAN_DLC_BYTES_3;
        case 4:  return FDCAN_DLC_BYTES_4;
        case 5:  return FDCAN_DLC_BYTES_5;
        case 6:  return FDCAN_DLC_BYTES_6;
        case 7:  return FDCAN_DLC_BYTES_7;
        case 8:  return FDCAN_DLC_BYTES_8;
        case 12: return FDCAN_DLC_BYTES_12;
        case 16: return FDCAN_DLC_BYTES_16;
        case 20: return FDCAN_DLC_BYTES_20;
        case 24: return FDCAN_DLC_BYTES_24;
        case 32: return FDCAN_DLC_BYTES_32;
        case 48: return FDCAN_DLC_BYTES_48;
        case 64: return FDCAN_DLC_BYTES_64;
        default: return FDCAN_DLC_BYTES_8;
    }
}

/* ================= SteeringCAN (fdcan2) ================= */
FDCAN_HandleTypeDef *steering_hfdcan = NULL;

can_status_t SteeringCAN_Init(void) {
	steering_hfdcan = hfdcan2;    
	if (steering_hfdcan == NULL) {
		return CAN_ERR;
	}

	steering_hfdcan->Instance = FDCAN2;
	steering_hfdcan->Init.ClockDivider = FDCAN_CLOCK_DIV1;
	steering_hfdcan->Init.FrameFormat = FDCAN_FRAME_CLASSIC;
	steering_hfdcan->Init.Mode = FDCAN_MODE_NORMAL;
	steering_hfdcan->Init.AutoRetransmission = ENABLE;
	steering_hfdcan->Init.TransmitPause = DISABLE;
	steering_hfdcan->Init.ProtocolException = DISABLE;
	steering_hfdcan->Init.NominalPrescaler = 10;
	steering_hfdcan->Init.NominalSyncJumpWidth = 1;
	steering_hfdcan->Init.NominalTimeSeg1 = 13;
	steering_hfdcan->Init.NominalTimeSeg2 = 2;
	steering_hfdcan->Init.DataPrescaler = 1;
	steering_hfdcan->Init.DataSyncJumpWidth = 1;
	steering_hfdcan->Init.DataTimeSeg1 = 1;
	steering_hfdcan->Init.DataTimeSeg2 = 1;
	steering_hfdcan->Init.StdFiltersNbr = 1;
	steering_hfdcan->Init.ExtFiltersNbr = 0;
	steering_hfdcan->Init.TxFifoQueueMode = FDCAN_TX_FIFO_OPERATION;

	FDCAN_FilterTypeDef sFilterConfig = {0};
	sFilterConfig.IdType = FDCAN_STANDARD_ID;
	sFilterConfig.FilterIndex = 0;
	sFilterConfig.FilterType = FDCAN_FILTER_MASK;
	sFilterConfig.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;

	// no filter yet
	sFilterConfig.FilterID1 = 0x00000000;
	sFilterConfig.FilterID2 = 0x00000000;

	if (can_fd_init(steering_hfdcan, &sFilterConfig) != CAN_OK) {
		return CAN_ERR;
	}

	if (can_fd_start(steering_hfdcan) != CAN_OK) {
		return CAN_ERR;
	}

	return CAN_OK;
}

can_status_t SteeringCAN_Send(uint32_t id, uint32_t payloadSize_dlc, uint8_t* data, TickType_t delay_ticks) {

	if(data == NULL || payloadSize_dlc > 64) {
		return CAN_ERR;
	}

	FDCAN_TxHeaderTypeDef steeringCAN_tx_header = {
		.Identifier = id,
		.IdType = FDCAN_STANDARD_ID,
		.TxFrameType = FDCAN_DATA_FRAME,
		.DataLength = fdcan_dlc_from_bytes(payloadSize_dlc),
		.ErrorStateIndicator = FDCAN_ESI_ACTIVE,
		.BitRateSwitch = FDCAN_BRS_OFF,
		.FDFormat = FDCAN_CLASSIC_CAN,
		.TxEventFifoControl = FDCAN_NO_TX_EVENTS,
		.MessageMarker = 0,
	};
	if (steering_hfdcan == NULL) {
		return CAN_ERR;
	}
	return can_fd_send(steering_hfdcan, &steeringCAN_tx_header, data, delay_ticks);
}

can_status_t SteeringCAN_ResetAngle(TickType_t delay_ticks) {
	
	uint8_t steering_tx_payload[CAN_DLC_LWS_CONFIG] = {0};
	// clear the CCW bits (bits 0-2)
	steering_tx_payload[0] &= ~(0x07);

	// set the CCW bits to 3 (0b011) to reset the angle
	steering_tx_payload[0] |= (LWS_CONFIG_LWS_CCW_SETS_THE_SIGNAL_LWS_ANGLE_TO_0Ã_Â_ & 0x07);

	return SteeringCAN_Send(CAN_ID_LWS_CONFIG, CAN_DLC_LWS_CONFIG, steering_tx_payload, delay_ticks);
}

can_status_t SteeringCAN_ResetCalibration(TickType_t delay_ticks){

	uint8_t steering_tx_payload[CAN_DLC_LWS_CONFIG] = {0};
	// clear the CCW bits (bits 0-2)
	steering_tx_payload[0] &= ~(0x07);

	// set the CCW bits to 3 (0b011) to reset the angle
	steering_tx_payload[0] |= (LWS_CONFIG_LWS_CCW_RESETS_CALIBRATION_STATUS & 0x07);

	return SteeringCAN_Send(CAN_ID_LWS_CONFIG, CAN_DLC_LWS_CONFIG, steering_tx_payload, delay_ticks);
}


void can_fd_rx_callback_hook(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs, can_rx_payload_t recv_payload) {

    if (steering_hfdcan != NULL && hfdcan->Instance == steering_hfdcan->Instance && CarCAN != NULL){
        BaseType_t higherPriorityTaskWoken = pdFALSE;

        // don't yield at the end of this since the rest of the ISR needs to run
        
        FDCAN_TxHeaderTypeDef tx_header = {0};   
        tx_header.Identifier = recv_payload.header.Identifier;
        tx_header.IdType = recv_payload.header.IdType;
        tx_header.TxFrameType = FDCAN_DATA_FRAME;
        tx_header.DataLength = recv_payload.header.DataLength;
        tx_header.ErrorStateIndicator = recv_payload.header.ErrorStateIndicator;
        tx_header.BitRateSwitch = recv_payload.header.BitRateSwitch;
        tx_header.FDFormat = recv_payload.header.FDFormat;
        tx_header.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
        tx_header.MessageMarker = 0;

        can_fd_send_isr(CarCAN, &tx_header, recv_payload.data, &higherPriorityTaskWoken);
    }
	// forward LightingCAN messages to CarCAN as well
	else if(LightingCAN != NULL && hfdcan->Instance == LightingCAN->Instance && CarCAN != NULL){
		BaseType_t higherPriorityTaskWoken = pdFALSE;

		// don't yield at the end of this since the rest of the ISR needs to run
		
		FDCAN_TxHeaderTypeDef tx_header = {0};   
		tx_header.Identifier = recv_payload.header.Identifier;
		tx_header.IdType = recv_payload.header.IdType;
		tx_header.TxFrameType = FDCAN_DATA_FRAME;
		tx_header.DataLength = recv_payload.header.DataLength;
		tx_header.ErrorStateIndicator = recv_payload.header.ErrorStateIndicator;
		tx_header.BitRateSwitch = recv_payload.header.BitRateSwitch;
		tx_header.FDFormat = recv_payload.header.FDFormat;
		tx_header.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
		tx_header.MessageMarker = 0;

		can_fd_send_isr(CarCAN, &tx_header, recv_payload.data, &higherPriorityTaskWoken);
	}
}