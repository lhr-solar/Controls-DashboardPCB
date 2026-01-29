#include "init.h"

int main(){
    HAL_Init();

    GPIO_Init();

    while(1){
        set_LED(PH_CAN_TX, GPIO_PIN_SET);
        HAL_Delay(500);
        set_LED(PH_CAN_TX, GPIO_PIN_RESET);
        HAL_Delay(500);
    }

    return 0;
}
