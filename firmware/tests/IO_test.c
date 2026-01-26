#include "stm32xx_hal.h"
#include "Inputs.h"

/* LSOM
    #define LED_PIN_OB GPIO_PIN_3
    #define LED_PORT_OB GPIOB
    #define LED_PIN GPIO_PIN_3 //3 LSOM
    #define LED_PORT GPIOC //C LSOM
*/
#define LED_PIN GPIO_PIN_10
#define LED_PORT GPIOC

#define INPUT_PIN GPIO_PIN_6
#define INPUT_PORT GPIOA


int main(){
    HAL_Init();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();

    GPIO_InitTypeDef led_config = {
        .Mode = GPIO_MODE_OUTPUT_PP,
        .Pull = GPIO_NOPULL,
        .Pin = LED_PIN
    };
    GPIO_InitTypeDef input_config = {
        .Mode = GPIO_MODE_INPUT,
        .Pull = GPIO_PULLDOWN,
        .Pin = INPUT_PIN
    };

    HAL_GPIO_Init(LED_PORT, &led_config); // initialize GPIOA with led_config
    HAL_GPIO_Init(INPUT_PORT, &input_config);
    HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_RESET);
    while(1){
        bool input_check = (HAL_GPIO_ReadPin(INPUT_PORT, INPUT_PIN) == 1);
        HAL_GPIO_WritePin(LED_PORT, LED_PIN, input_check ? GPIO_PIN_SET : GPIO_PIN_RESET);
    }

    return 0;
}
