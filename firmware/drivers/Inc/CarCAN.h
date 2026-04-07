#pragma once

#include <stdint.h>
#include "stm32xx_hal.h"
#include "pinDefs.h"
#include "CAN_FD.h"
#include "CarCAN_can_msgs.h"

/** CAN IDs and DLCs
 * CAN_ID_CONTROLS_STATUS
 * CAN_DLC_CONTROLS_STATUS
 * CAN_ID_DRIVER_INPUT_STATUS
 * CAN_DLC_DRIVER_INPUT_STATUS
*/

/**	BPS fault status enum
 * BPS_STATUS_BPS_FAULT_OK = YAY
 * other values for BPS = bad
 */

/** helpful enums
 * controls_status_controls_leader_fault_e
 * controls_status_lightingboard_front_status_e
 * controls_status_lightingboard_left_status_e
 * controls_status_lightingboard_right_status_e
 * controls_status_lightingboard_rear_status_e
 * controls_status_lightingboard_canopy_status_e
 * 
 * driver_input_status_ignition_array_e
 * driver_input_status_ignition_motor_e
 * driver_input_status_ignition_off_e
 * driver_input_status_cruise_enable_e
 * driver_input_status_cruise_set_e
 * driver_input_status_gear_forward_e
 * driver_input_status_gear_neutral_e
 * driver_input_status_hazard_pressed_e
 * driver_input_status_horn_pressed_e
 * driver_input_status_blinker_left_e
 * driver_input_status_blinker_right_e
 * driver_input_status_regen_activate_e
 * driver_input_status_regen_enable_e
 */


 /**helpful structs
  * controls_status_t
  * driver_input_status_t
  * 
  */

can_status_t CarCAN_Init(void);

can_status_t CarCAN_Send(uint32_t id, uint8_t data[8], TickType_t delay_ticks);

//can_status_t CarCAN_Receive(uint32_t *id_out, uint8_t data[8], TickType_t delay_ticks);

//void CarCAN_Unpack_CL_Payload(const uint8_t data[8], CarCAN_BPS_Aggregate_t *agg);