#include "init.h"
#include "Switches.h"
#include "Horn.h"
#include "Status_LEDs.h"
#include "pinDefs.h"
#include "stm32xx_hal.h"

static uint32_t HAL_RCC_FDCAN_CLK_ENABLED = 0;

void gpioPin_Init(GPIO_TypeDef *port, uint16_t pin, uint32_t mode) {
    GPIO_InitTypeDef GPIO_init = {
        .Mode = mode,
        .Pull = GPIO_NOPULL,
        .Pin  = pin
    };

	switch((uint32_t)port) {
		case (uint32_t)GPIOA: __HAL_RCC_GPIOA_CLK_ENABLE(); break;
		case (uint32_t)GPIOB: __HAL_RCC_GPIOB_CLK_ENABLE(); break;
		case (uint32_t)GPIOC: __HAL_RCC_GPIOC_CLK_ENABLE(); break;
		case (uint32_t)GPIOD: __HAL_RCC_GPIOD_CLK_ENABLE(); break;
		case (uint32_t)GPIOE: __HAL_RCC_GPIOE_CLK_ENABLE(); break;
		case (uint32_t)GPIOF: __HAL_RCC_GPIOF_CLK_ENABLE(); break;
    	default: break;
	}

    HAL_GPIO_Init(port, &GPIO_init);
    if (mode != GPIO_MODE_OUTPUT_PP) HAL_GPIO_WritePin(port, pin, GPIO_PIN_RESET);
}

void SystemClock_Config(void) {
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV1;
  RCC_OscInitStruct.PLL.PLLN = 20;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}



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
        HAL_NVIC_SetPriority(FDCAN3_IT0_IRQn, FDCAN_NVIC_PRIO, 0);
        HAL_NVIC_EnableIRQ(FDCAN3_IT0_IRQn);
        HAL_NVIC_SetPriority(FDCAN3_IT1_IRQn, FDCAN_NVIC_PRIO, 0);
        HAL_NVIC_EnableIRQ(FDCAN3_IT1_IRQn);
        // ElconCAN: PA8 (RX), PA15 (TX), AF11
        GPIO_InitStruct.Pin = CAR_CAN_TX_PIN | CAR_CAN_RX_PIN;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF11_FDCAN3;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    }

    // steering CAN
    if(fdcanHandle->Instance == FDCAN2)
    {

         __HAL_RCC_GPIOB_CLK_ENABLE();


        /**FDCAN2 GPIO Configuration
        PB12     ------> FDCAN2_RX
        PB13     ------> FDCAN2_TX
        */
        HAL_NVIC_SetPriority(FDCAN2_IT0_IRQn, FDCAN_NVIC_PRIO, 0);
        HAL_NVIC_EnableIRQ(FDCAN2_IT0_IRQn);
        HAL_NVIC_SetPriority(FDCAN2_IT1_IRQn, FDCAN_NVIC_PRIO, 0);
        HAL_NVIC_EnableIRQ(FDCAN2_IT1_IRQn);
        GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF9_FDCAN2;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    }

	if(fdcanHandle->Instance == FDCAN1)
	{
		HAL_NVIC_SetPriority(FDCAN1_IT0_IRQn, FDCAN_NVIC_PRIO, 0);
		HAL_NVIC_EnableIRQ(FDCAN1_IT0_IRQn);
		HAL_NVIC_SetPriority(FDCAN1_IT1_IRQn, FDCAN_NVIC_PRIO, 0);
		HAL_NVIC_EnableIRQ(FDCAN1_IT1_IRQn);
		// LightingCAN: PA11 (RX), PA12 (TX), AF9
		GPIO_InitStruct.Pin = LIGHTING_CAN_TX_PIN | LIGHTING_CAN_RX_PIN;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		GPIO_InitStruct.Alternate = GPIO_AF9_FDCAN1;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	}
}
