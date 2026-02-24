#include "init.h"

int main() {
    HAL_Init();
    GPIO_Init();
    while(1) {
        flash_them();
    }

    return 0;
}