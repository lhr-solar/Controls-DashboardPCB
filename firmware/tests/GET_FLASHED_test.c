#include "init.h"

int main() {
    HAL_Init();
    GPIO_Init();

    flash_them();

    return 0;
}