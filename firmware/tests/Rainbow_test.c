#include "init.h"

int main() {
    HAL_Init();
    GPIO_Init();

    while(1) {
        RainbowLED();
    }
    
    return 0;
}