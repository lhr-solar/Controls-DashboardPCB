#include "stm32xx_hal.h"
#include "Inputs.h"
#include <stdbool.h>


/* LSOM
    #define AKSHAY_LED_PIN_OB GPIO_PIN_3
    #define AKSHAY_LED_PORT_OB GPIOB
    #define AKSHAY_LED_PIN GPIO_PIN_3 //3 LSOM
    #define AKSHAY_LED_PORT GPIOC //C LSOM
*/
#define AKSHAY_LED_PIN GPIO_PIN_10
#define AKSHAY_LED_PORT GPIOC
#define X_LED2_PIN GPIO_PIN_14
#define X_LED2_PORT GPIOB
#define Heartbeat_PIN GPIO_PIN_3
#define Heartbeat_PORT GPIOC

#define IGN_MTR_PIN GPIO_PIN_1
#define IGN_MTR_PORT GPIOA
#define FWD_SW_PIN GPIO_PIN_0
#define FWD_SW_PORT GPIOA


int main(){
    HAL_Init();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();

    GPIO_InitTypeDef led_config = {
        .Mode = GPIO_MODE_OUTPUT_PP,
        .Pull = GPIO_NOPULL,
        .Pin = AKSHAY_LED_PIN
    };
    GPIO_InitTypeDef led2_config = {
        .Mode = GPIO_MODE_OUTPUT_PP,
        .Pull = GPIO_NOPULL,
        .Pin = X_LED2_PIN
    };
    GPIO_InitTypeDef hb_config = {
        .Mode = GPIO_MODE_OUTPUT_PP,
        .Pull = GPIO_NOPULL,
        .Pin = Heartbeat_PIN
    };
    GPIO_InitTypeDef hazard_config = {
        .Mode = GPIO_MODE_INPUT,
        .Pull = GPIO_NOPULL,
        .Pin = IGN_MTR_PIN
    };
    GPIO_InitTypeDef fwd_sw_config = {
        .Mode = GPIO_MODE_INPUT,
        .Pull = GPIO_NOPULL,
        .Pin = FWD_SW_PIN
    };

    HAL_GPIO_Init(AKSHAY_LED_PORT, &led_config); // initialize GPIOA with led_config
    HAL_GPIO_Init(X_LED2_PORT, &led2_config);
    HAL_GPIO_Init(Heartbeat_PORT, &hb_config);
    HAL_GPIO_Init(IGN_MTR_PORT, &hazard_config);
    HAL_GPIO_Init(IGN_MTR_PORT, &fwd_sw_config);
    HAL_GPIO_WritePin(AKSHAY_LED_PORT, AKSHAY_LED_PIN, GPIO_PIN_RESET);
    while (1) {
        GPIO_PinState hazard = HAL_GPIO_ReadPin(IGN_MTR_PORT, IGN_MTR_PIN);
        GPIO_PinState fwd_sw = HAL_GPIO_ReadPin(FWD_SW_PORT, FWD_SW_PIN);

        bool hazard_active = (hazard == GPIO_PIN_RESET);
        bool fwd_sw_active = (fwd_sw == GPIO_PIN_RESET);
        HAL_GPIO_WritePin(AKSHAY_LED_PORT, AKSHAY_LED_PIN,
                          hazard_active ? GPIO_PIN_SET : GPIO_PIN_RESET);
        HAL_GPIO_WritePin(X_LED2_PORT, X_LED2_PIN,
                          fwd_sw_active ? GPIO_PIN_SET : GPIO_PIN_RESET);

    }


    return 0;
}
