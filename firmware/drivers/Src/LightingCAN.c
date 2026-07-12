#include "LightingCAN.h"
#include "Horn.h"
#include "Status_LEDs.h"
#include "Switches.h"
#include "init.h"
#include "LightingCAN_can_msgs.h"
#include "CarCAN.h"

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

FDCAN_HandleTypeDef *LightingCAN = NULL;
FDCAN_RxHeaderTypeDef LightingCAN_rx_header;

can_status_t LightingCAN_Init(void) {
	LightingCAN = hfdcan1;
	if (LightingCAN == NULL) {
		return CAN_ERR;
	}

	 __HAL_RCC_SYSCFG_CLK_ENABLE();
    __HAL_RCC_PWR_CLK_ENABLE();

	LightingCAN->Instance = FDCAN1;
	LightingCAN->Init.ClockDivider = FDCAN_CLOCK_DIV1;
	LightingCAN->Init.FrameFormat = FDCAN_FRAME_CLASSIC;
	LightingCAN->Init.Mode = FDCAN_MODE_NORMAL;
	LightingCAN->Init.AutoRetransmission = ENABLE;
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

can_status_t LightingCAN_SendLightingCommand(lighting_command_t command, TickType_t delay_ticks) {
	
	uint8_t tx_payload[CAN_DLC_LIGHTING_COMMAND] = {0};
	// Lighting_Set_Headlights is bit 0 
	tx_payload[0] |= (command.Lighting_Set_Headlights & 0x1) << 0;
	// Lighting_Set_Left_Indicator is bit 1
	tx_payload[0] |= (command.Lighting_Set_Left_Indicator & 0x1) << 1;
	// Lighting_Set_Right_Indicator is bit 2
	tx_payload[0] |= (command.Lighting_Set_Right_Indicator & 0x1) << 2;
	// Lighting_Blink_Sync is bit 3
	tx_payload[0] |= (command.Lighting_Blink_Sync & 0x1) << 3;
	// Lighting_Set_Brake is bit 4
	tx_payload[0] |= (command.Lighting_Set_Brake & 0x1) << 4;
	// Lighting_Set_BPS_Strobe is bit 5
	tx_payload[0] |= (command.Lighting_Set_BPS_Strobe & 0x1) << 5;
	// Lighting_Set_Custom_Mode is bits 6-7
	tx_payload[0] |= (command.Lighting_Set_Custom_Mode & 0x3) << 6;

	can_status_t lighting_can_send_status = LightingCAN_Send(CAN_ID_LIGHTING_COMMAND, CAN_DLC_LIGHTING_COMMAND, tx_payload, delay_ticks);

	// also forward the lighting command to CarCAN
	can_status_t car_can_send_status = CarCAN_Send(CAN_ID_LIGHTING_COMMAND, CAN_DLC_LIGHTING_COMMAND, tx_payload, delay_ticks);
	return lighting_can_send_status == CAN_OK && car_can_send_status == CAN_OK;
}

can_status_t LightingCAN_Send(uint32_t id, uint32_t payloadSize_dlc, uint8_t* data, TickType_t delay_ticks) {
	FDCAN_TxHeaderTypeDef LightingCAN_tx_header = {
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

	if ((LightingCAN == NULL) || (data == NULL)) {
		return CAN_ERR;
	}
	if (can_fd_send(LightingCAN, &LightingCAN_tx_header, data, delay_ticks) == CAN_ERR) {
		return CAN_ERR;
	}

	led_toggle(PH_CAN_TX_LED_PORT, PH_CAN_TX_LED_PIN);
	return CAN_OK;
}




