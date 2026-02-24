#include "init.h"

int main() {
    HAL_Init();
    GPIO_Init();

    while(1) {
        set_LED(LSOM_HB, GPIO_PIN_SET);
        HAL_Delay(500);
        set_LED(LSOM_HB, GPIO_PIN_RESET);
        HAL_Delay(500);
        set_Horn(get_switch_state(Horn));
    }

    return 0;
}