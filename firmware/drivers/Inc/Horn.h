#pragma once

#include "Switches.h"
#include "pinDefs.h"

/**
 * @brief  Initializes the horn output GPIO pin as a push-pull output
 */
void horn_gpio_init();

/**
 * @brief  Sets the horn output to the specified state.
 *         Also mirrors the state to AKSHAY_LED for debugging.
 * @param  state  SWITCH_ON to activate the horn, SWITCH_OFF to deactivate.
 */
void horn_set(switch_state_t state);