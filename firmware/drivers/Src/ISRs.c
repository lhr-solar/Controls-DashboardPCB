#include "init.h"
#include "Status_LEDs.h"
#include "Horn.h"
#include "Switches.h"

/**
 * @brief  EXTI callback fired on any configured external interrupt edge.
 *         Handles debounce, then updates LEDs based on switch states.
 *         - NEUTRAL_GEAR: updates PH_CAN_TX_LED, and CAR_CAN_TX_LED if FWD or REV is active.
 *         - IGN_OFF:      updates PH_CAN_RX_LED to mirror ignition off switch state.
 * 
 * @param  GPIO_Pin  Pin number that triggered the interrupt.
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    if (GPIO_Pin == NEUTRAL_GEAR_PIN) {
        led_set(PH_CAN_TX_LED_PORT, PH_CAN_TX_LED_PIN, switch_get_state(NEUTRAL_GEAR_PORT, NEUTRAL_GEAR_PIN));
        if (switch_get_state(FWD_SW_PORT, FWD_SW_PIN) == ON) {
            led_set(CAR_CAN_TX_LED_PORT, CAR_CAN_TX_LED_PIN, ON);
        } else if (switch_get_state(REV_SW_PORT, REV_SW_PIN) == ON) {
            led_set(CAR_CAN_TX_LED_PORT, CAR_CAN_TX_LED_PIN, ON);
        } else {
            led_set(PH_CAN_TX_LED_PORT, PH_CAN_TX_LED_PIN, switch_get_state(NEUTRAL_GEAR_PORT, NEUTRAL_GEAR_PIN));
        }
    } else if (GPIO_Pin == IGN_OFF_PIN) {
        led_set(PH_CAN_RX_LED_PORT, PH_CAN_RX_LED_PIN, switch_get_state(IGN_OFF_PORT, IGN_OFF_PIN));
    }

	/**
	 * 
	 * @todo 	write Neautral Gear and Ign Off States to the switch states payload
	 * 
	*/
}

/**
 * @brief  EXTI line 0 interrupt handler. Routed to NEUTRAL_GEAR_PIN.
 */
void EXTI0_IRQHandler(void) {
    HAL_GPIO_EXTI_IRQHandler(NEUTRAL_GEAR_PIN);
}

/**
 * @brief  EXTI line 2 interrupt handler. Routed to IGN_OFF_PIN.
 */
void EXTI2_IRQHandler(void) {
    HAL_GPIO_EXTI_IRQHandler(IGN_OFF_PIN);
}