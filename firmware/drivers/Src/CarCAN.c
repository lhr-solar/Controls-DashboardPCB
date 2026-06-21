#include "CarCAN.h"
#include "Horn.h"
#include "Status_LEDs.h"
#include "Switches.h"
#include "init.h"

/* ================= SteeringCAN (fdcan2) ================= */
FDCAN_HandleTypeDef *steering_hfdcan = NULL;

/* ================= CarCAN (fdcan3) ================= */
static FDCAN_HandleTypeDef *CarCAN = NULL;
static FDCAN_RxHeaderTypeDef carCAN_rx_header;

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
	steering_hfdcan->Init.NominalPrescaler = 20;
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


void can_fd_rx_callback_hook(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs, can_rx_payload_t recv_payload ){
    
    // only forward motorCAN messages to CarCAN
    if (steering_hfdcan != NULL && hfdcan->Instance == steering_hfdcan->Instance){

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