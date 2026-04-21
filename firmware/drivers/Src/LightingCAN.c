#include "LightingCAN.h"
#include "Horn.h"
#include "Status_LEDs.h"
#include "Switches.h"
#include "init.h"

FDCAN_HandleTypeDef *LightingCAN = NULL;
FDCAN_RxHeaderTypeDef LightingCAN_rx_header;


can_status_t LightingCAN_Init(void) {
	LightingCAN = hfdcan1;

	LightingCAN->Instance = FDCAN1;
	LightingCAN->Init.ClockDivider = FDCAN_CLOCK_DIV1;
	LightingCAN->Init.FrameFormat = FDCAN_FRAME_CLASSIC;
	LightingCAN->Init.Mode = FDCAN_MODE_NORMAL;
	LightingCAN->Init.AutoRetransmission = DISABLE;
	LightingCAN->Init.TransmitPause = DISABLE;
	LightingCAN->Init.ProtocolException = DISABLE;
	LightingCAN->Init.NominalPrescaler = 20;
	LightingCAN->Init.NominalSyncJumpWidth = 1;
	LightingCAN->Init.NominalTimeSeg1 = 13;
	LightingCAN->Init.NominalTimeSeg2 = 2;
	LightingCAN->Init.DataPrescaler = 1;
	LightingCAN->Init.DataSyncJumpWidth = 1;
	LightingCAN->Init.DataTimeSeg1 = 1;
	LightingCAN->Init.DataTimeSeg2 = 1;
	LightingCAN->Init.StdFiltersNbr = 1;
	LightingCAN->Init.ExtFiltersNbr = 0;
	LightingCAN->Init.TxFifoQueueMode = FDCAN_TX_FIFO_OPERATION;

	FDCAN_FilterTypeDef sFilterConfig = {0};
	sFilterConfig.IdType = FDCAN_STANDARD_ID;
	sFilterConfig.FilterIndex = 0;
	sFilterConfig.FilterType = FDCAN_FILTER_MASK;
	sFilterConfig.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;

	// no filter yet
	sFilterConfig.FilterID1 = 0x00000000;
	sFilterConfig.FilterID2 = 0x00000000;

	if (can_fd_init(LightingCAN, &sFilterConfig) != CAN_OK) {
		return CAN_ERR;
	}

	if (can_fd_start(LightingCAN) != CAN_OK) {
		return CAN_ERR;
	}

	return CAN_OK;
}

can_status_t LightingCAN_Send(uint32_t id, uint32_t payloadSize_dlc, uint8_t* data, TickType_t delay_ticks) {
	FDCAN_TxHeaderTypeDef LightingCAN_tx_header = {
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

	if(LightingCAN == NULL) return CAN_ERR;
	if (can_fd_send(LightingCAN, &LightingCAN_tx_header, data, delay_ticks) == CAN_ERR) {
		led_set(PH_CAN_TX_LED_PORT, PH_CAN_TX_LED_PIN, LED_ON);
		return CAN_ERR;
	}

	led_toggle(PH_CAN_TX_LED_PORT, PH_CAN_TX_LED_PIN);
	return CAN_OK;
}


