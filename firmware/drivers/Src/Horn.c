#include "init.h"
#include "Horn.h"
#include "Switches.h"

void horn_gpio_init() {
    gpioPin_Init(HORN_OUT_PORT, HORN_OUT_PIN, GPIO_MODE_OUTPUT_PP);
	horn_set(SWITCH_OFF);
}

void horn_set(switch_state_t state) {
    // Output stage (Q6) is active-high at the gate: gate HIGH = horn on.
    // switch_state_t is active-low (SWITCH_ON = GPIO_PIN_RESET), so invert here.
    HAL_GPIO_WritePin(HORN_OUT_PORT, HORN_OUT_PIN,
                      (state == SWITCH_ON) ? GPIO_PIN_SET : GPIO_PIN_RESET);
}