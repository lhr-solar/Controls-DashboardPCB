#include "init.h"

int main() {
    HAL_Init();
    LED_GPIO_Init();
    Switch_GPIO_Init();
    EXTI_Init(Neutral_Gear, 5);
    EXTI_Init(Ign_OFF, 4);

    while(1) {
        set_LED(LSOM_HB, GPIO_PIN_SET);
        HAL_Delay(1000);
        set_LED(LSOM_HB, GPIO_PIN_RESET);
        HAL_Delay(1000);
    }

    return 0;
}