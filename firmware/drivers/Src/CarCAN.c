#include "CarCAN.h"
#include "CAN_FD.h"
#include "Horn.h"
#include "Status_LEDs.h"
#include "Switches.h"
#include "init.h"
#include "stm32xx_hal.h"

/* ================= CarCAN (fdcan3) ================= */
static uint32_t HAL_RCC_FDCAN_CLK_ENABLED = 0;

// Single MspInit handles both FDCAN instances — HAL only allows one definition project-wide
void HAL_FDCAN_MspInit(FDCAN_HandleTypeDef *fdcanHandle)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

    // Shared clock source config (only needs to run once, same source for all FDCAN)
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_FDCAN;
    PeriphClkInit.FdcanClockSelection = RCC_FDCANCLKSOURCE_PCLK1;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
        Error_Handler();
    }

    HAL_RCC_FDCAN_CLK_ENABLED++;
    if (HAL_RCC_FDCAN_CLK_ENABLED == 1)
    {
        __HAL_RCC_FDCAN_CLK_ENABLE();
    }

    __HAL_RCC_GPIOA_CLK_ENABLE();

    if (fdcanHandle->Instance == FDCAN3)
    {
        HAL_NVIC_SetPriority(FDCAN3_IT0_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY, 0);
        HAL_NVIC_EnableIRQ(FDCAN3_IT0_IRQn);
        HAL_NVIC_SetPriority(FDCAN3_IT1_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY, 0);
        HAL_NVIC_EnableIRQ(FDCAN3_IT1_IRQn);
        // ElconCAN: PA8 (RX), PA15 (TX), AF11
        GPIO_InitStruct.Pin = CAR_CAN_TX_PIN | CAR_CAN_RX_PIN;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF11_FDCAN3;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    }
}


static FDCAN_HandleTypeDef *CarCAN = NULL;

static FDCAN_TxHeaderTypeDef carCAN_tx_header = {
	.Identifier = CAN_ID_DRIVER_INPUT_STATUS,
	.IdType = FDCAN_STANDARD_ID,
	.TxFrameType = FDCAN_DATA_FRAME,
	.DataLength = FDCAN_DLC_BYTES_8,
	.ErrorStateIndicator = FDCAN_ESI_ACTIVE,
	.BitRateSwitch = FDCAN_BRS_OFF,
	.FDFormat = FDCAN_CLASSIC_CAN,
	.TxEventFifoControl = FDCAN_NO_TX_EVENTS,
	.MessageMarker = 0,
};


can_status_t CarCAN_Init(void) {
	CarCAN = hfdcan3;

	CarCAN->Instance = FDCAN3;
	CarCAN->Init.ClockDivider = FDCAN_CLOCK_DIV1;
	CarCAN->Init.FrameFormat = FDCAN_FRAME_CLASSIC;
	CarCAN->Init.Mode = FDCAN_MODE_NORMAL;
	CarCAN->Init.AutoRetransmission = DISABLE;
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

can_status_t CarCAN_Send(uint32_t id, uint8_t data[8], TickType_t delay_ticks) {
	carCAN_tx_header.Identifier = id;

	if (can_fd_send(CarCAN, &carCAN_tx_header, data, delay_ticks) == CAN_ERR) {
		led_set(CAR_CAN_TX_LED_PORT, CAR_CAN_TX_LED_PIN, LED_OFF);
		return CAN_ERR;
	}

	// Blink to indicate activity rather than holding ON
	led_toggle(CAR_CAN_TX_LED_PORT, CAR_CAN_TX_LED_PIN);
	return CAN_OK;
}

// can_status_t CarCAN_Receive(uint32_t *id_out, uint8_t data[8],
//							TickType_t delay_ticks) {
//	static const uint32_t CL_ids[] = {CAN_ID_DRIVER_INPUT_STATUS,
// CAN_ID_CONTROLS_STATUS}; 	TickType_t ticks = delay_ticks; 	can_status_t
// result = can_fd_recv(CarCAN, CL_ids[0], &carCAN_rx_header, data, ticks);
// if (result == CAN_OK) { 		*id_out = CL_ids[0]; 		return CAN_OK;
//	}
//	return CAN_EMPTY;
// }

// void CarCAN_Unpack_CL_Payload(const uint8_t data[8],
//							  CarCAN_BPS_Aggregate_t *agg) {
//	uint8_t idx = data[0] & 0x1F;
//	if (idx >= BPS_TAP_COUNT) {
//		return;
//	}

//	// BPS_Voltage_Tap_Data: start bit 8, length 16, scale 0.001
//	uint16_t raw_v = (uint16_t)data[1] | ((uint16_t)data[2] << 8);
//	agg->taps[idx].voltage = raw_v * 0.001f;

//	// BPS_Temperature_Tap_Data: start bit 24, length 32, scale 0.001
//	int32_t raw_t = (int32_t)data[3] | ((int32_t)data[4] << 8) |
//					((int32_t)data[5] << 16) | ((int32_t)data[6] << 24);
//	agg->taps[idx].temperature = raw_t * 0.001f;

//	agg->last_updated_idx = idx;
//}