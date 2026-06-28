#include "SteeringCAN.h"

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


void can_fd_rx_callback_hook(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs, can_rx_payload_t recv_payload) {

    // only forward motorCAN messages to CarCAN
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
}