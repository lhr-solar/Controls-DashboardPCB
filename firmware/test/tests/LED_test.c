#include "stm32xx_hal.h"

// Heartbeat pin on stm32l432kcu is PB3
#ifdef STM32L432xx
    #define LED_PIN GPIO_PIN_3
    #define LED_PORT GPIOB
#elif STM32L431xx
    #define LED_PIN GPIO_PIN_11
    #define LED_PORT GPIOB
#else
// Most other nucleos have a heartbeat for A5
    #define LED_PIN_OB GPIO_PIN_3
    #define LED_PORT_OB GPIOB
    #define LED_PIN GPIO_PIN_5 //4 LSOM
    #define LED_PORT GPIOA //B LSOM
#endif


// Initialize clock for heartbeat LED port
void Heartbeat_Clock_Init() {
    switch ((uint32_t)LED_PORT) {
        case (uint32_t)GPIOA:
            __HAL_RCC_GPIOA_CLK_ENABLE();
            break;
        case (uint32_t)GPIOB:
            __HAL_RCC_GPIOB_CLK_ENABLE();
            break;
        case (uint32_t)GPIOC:
            __HAL_RCC_GPIOC_CLK_ENABLE();
            break;
    }
    switch ((uint32_t)LED_PORT_OB) {
        case (uint32_t)GPIOA:
            __HAL_RCC_GPIOA_CLK_ENABLE();
            break;
        case (uint32_t)GPIOB:
            __HAL_RCC_GPIOB_CLK_ENABLE();
            break;
        case (uint32_t)GPIOC:
            __HAL_RCC_GPIOC_CLK_ENABLE();
            break;
    }
}

int main(){
    HAL_Init();

    GPIO_InitTypeDef led_config = {
        .Mode = GPIO_MODE_OUTPUT_PP,
        .Pull = GPIO_NOPULL,
        .Pin = LED_PIN
    };

    GPIO_InitTypeDef led_config_OB = {
        .Mode = GPIO_MODE_OUTPUT_PP,
        .Pull = GPIO_NOPULL,
        .Pin = LED_PIN_OB
    };
    
    Heartbeat_Clock_Init(); // enable clock for LED_PORT
    HAL_GPIO_Init(LED_PORT, &led_config); // initialize GPIOA with led_config
    HAL_GPIO_Init(LED_PORT_OB, &led_config_OB);

    while(1){
        HAL_GPIO_TogglePin(LED_PORT, LED_PIN);
        HAL_GPIO_TogglePin(LED_PORT_OB, LED_PIN_OB);
        HAL_Delay(500);
    }

    return 0;
}
