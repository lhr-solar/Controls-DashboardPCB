#include "stm32xx_hal.h"
#include "Inputs.h"

/* LSOM
    #define LED_PIN_OB GPIO_PIN_3
    #define LED_PORT_OB GPIOB
    #define LED_PIN GPIO_PIN_3 //3 LSOM
    #define LED_PORT GPIOC //C LSOM
*/

uint16_t input_state[INPUT_STATE_COUNT];
uint16_t led_state[LED_COUNT];
#define LED_PIN GPIO_PIN_10
#define LED_PORT GPIOC
#define LED_PIN2 GPIO_PIN_11
#define LED_PORT2 GPIOC

#define INPUT_PIN GPIO_PIN_6
#define INPUT_PORT GPIOA

void update_leds();
void logic(Input_Indexes input);


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


}

void init(void) {
    for (int i = 0; i < INPUT_STATE_COUNT; i++) {
        input_state[i] = 0;
    }
    for (int i = 0; i < LED_COUNT; i++) {
        led_state[i] = 0;
    }
    update_leds();
}

void update_inputs(void) {
    for (int i = 0; i < INPUT_STATE_COUNT; i++) {
        GPIO_Pin pin = gpio_map[i];
        input_state[i] = HAL_GPIO_ReadPin(pin.port, pin.pin);
        logic((Input_Indexes)i);
    }
}

void logic(Input_Indexes input) {
    if (input_state[input] == 1) {
        led_state[AKSHAY_LED] = 1;
    } else {
        led_state[AKSHAY_LED] = 0;
    }
}

void update_leds(void) {
    for (int i = 0; i < LED_COUNT; i++) {

        GPIO_Pin pin = gpio_map[AKSHAY_LED_PIN + i];

        HAL_GPIO_WritePin(pin.port, pin.pin, led_state[i] ? GPIO_PIN_SET : GPIO_PIN_RESET);
    }
}


int main(){
    HAL_Init();

    GPIO_InitTypeDef led_config = {
        .Mode = GPIO_MODE_OUTPUT_PP,
        .Pull = GPIO_NOPULL,
        .Pin = LED_PIN
    };
    GPIO_InitTypeDef led_config2 = {
        .Mode = GPIO_MODE_OUTPUT_PP,
        .Pull = GPIO_NOPULL,
        .Pin = LED_PIN2
    };

    
    Heartbeat_Clock_Init(); // enable clock for LED_PORT
    HAL_GPIO_Init(LED_PORT, &led_config); // initialize GPIOA with led_config
    HAL_GPIO_Init(LED_PORT2, &led_config2); // initialize GPIOA with led_config
    HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LED_PORT2, LED_PIN2, GPIO_PIN_RESET);
    while(1){
        bool input_check = (HAL_GPIO_ReadPin(INPUT_PORT, INPUT_PIN) == 0);
        HAL_GPIO_WritePin(LED_PORT, LED_PIN, input_check ? GPIO_PIN_SET : GPIO_PIN_RESET);
        HAL_GPIO_WritePin(LED_PORT2, LED_PIN2, input_check ? GPIO_PIN_RESET : GPIO_PIN_SET);
    }

    return 0;
}
