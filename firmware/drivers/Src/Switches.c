#include "Switches.h"

SwitchState get_switch_state(GPIO_Pin sw) {
    GPIO_PinState switchState = HAL_GPIO_ReadPin(sw.port, sw.pin);
    if(switchState == GPIO_PIN_SET) return ON;
    return OFF;
}