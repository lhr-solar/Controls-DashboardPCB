#include "init.h"

int main() {
    HAL_Init();
    GPIO_Init();

    while(1) {
        toggle_LED(LSOM_HB);
        set_LED(X_LED2, get_switch_state(Ign_OFF));
        HAL_Delay(500);
    }

    return 0;
}
