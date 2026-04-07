#pragma once

#include <stdint.h>

/* ================= CAN ID Macros ================= */

#define CAN_ID_LIGHTING_COMMAND 0x660
#define CAN_ID_LIGHTING_FRONT_STATUS 0x670
#define CAN_ID_LIGHTING_LEFT_STATUS 0x671
#define CAN_ID_LIGHTING_REAR_STATUS 0x672
#define CAN_ID_LIGHTING_RIGHT_STATUS 0x673
#define CAN_ID_LIGHTING_CANOPY_STATUS 0x674

/* ================= CAN Length Macros ================= */

#define CAN_DLC_LIGHTING_COMMAND 1
#define CAN_DLC_LIGHTING_FRONT_STATUS 8
#define CAN_DLC_LIGHTING_LEFT_STATUS 8
#define CAN_DLC_LIGHTING_REAR_STATUS 8
#define CAN_DLC_LIGHTING_RIGHT_STATUS 8
#define CAN_DLC_LIGHTING_CANOPY_STATUS 8


/* ================= Value Table Enums ================= */

typedef enum {
    LIGHTING_COMMAND_LIGHTING_SET_HEADLIGHTS_ON = 1,
    LIGHTING_COMMAND_LIGHTING_SET_HEADLIGHTS_OFF = 0,
} lighting_command_lighting_set_headlights_e;

typedef enum {
    LIGHTING_COMMAND_LIGHTING_SET_LEFT_INDICATOR_ON = 1,
    LIGHTING_COMMAND_LIGHTING_SET_LEFT_INDICATOR_OFF = 0,
} lighting_command_lighting_set_left_indicator_e;

typedef enum {
    LIGHTING_COMMAND_LIGHTING_SET_RIGHT_INDICATOR_ON = 1,
    LIGHTING_COMMAND_LIGHTING_SET_RIGHT_INDICATOR_OFF = 0,
} lighting_command_lighting_set_right_indicator_e;

typedef enum {
    LIGHTING_COMMAND_LIGHTING_SET_BRAKE_ON = 1,
    LIGHTING_COMMAND_LIGHTING_SET_BRAKE_OFF = 0,
} lighting_command_lighting_set_brake_e;

typedef enum {
    LIGHTING_COMMAND_LIGHTING_SET_BPS_STROBE_ON = 1,
    LIGHTING_COMMAND_LIGHTING_SET_BPS_STROBE_OFF = 0,
} lighting_command_lighting_set_bps_strobe_e;

typedef enum {
    LIGHTING_COMMAND_LIGHTING_SET_CUSTOM_MODE_MODE_3_ACTIVE = 3,
    LIGHTING_COMMAND_LIGHTING_SET_CUSTOM_MODE_MODE_2_ACTIVE = 2,
    LIGHTING_COMMAND_LIGHTING_SET_CUSTOM_MODE_MODE_1_ACTIVE = 1,
    LIGHTING_COMMAND_LIGHTING_SET_CUSTOM_MODE_OFF = 0,
} lighting_command_lighting_set_custom_mode_e;

typedef enum {
    LIGHTING_FRONT_STATUS_LIGHTING_BOARD_FAULT_WATCHDOG = 8,
    LIGHTING_FRONT_STATUS_LIGHTING_BOARD_FAULT_LIGHT_COMMAND_WATCHDOG = 7,
    LIGHTING_FRONT_STATUS_LIGHTING_BOARD_FAULT_LED1_OVERCURRENT = 6,
    LIGHTING_FRONT_STATUS_LIGHTING_BOARD_FAULT_LED0_OVERCURRENT = 5,
    LIGHTING_FRONT_STATUS_LIGHTING_BOARD_FAULT_ADDR_LED_OVERCURRENT = 4,
    LIGHTING_FRONT_STATUS_LIGHTING_BOARD_FAULT_LED1_UNDERCURRENT = 3,
    LIGHTING_FRONT_STATUS_LIGHTING_BOARD_FAULT_LED0_UNDERCURRENT = 2,
    LIGHTING_FRONT_STATUS_LIGHTING_BOARD_FAULT_ADDR_LED_UNDERCURRENT = 1,
    LIGHTING_FRONT_STATUS_LIGHTING_BOARD_FAULT_OK = 0,
} lighting_front_status_lighting_board_fault_e;

typedef enum {
    LIGHTING_FRONT_STATUS_LIGHT_HEADLIGHT_ON = 1,
    LIGHTING_FRONT_STATUS_LIGHT_HEADLIGHT_OFF = 0,
} lighting_front_status_light_headlight_e;

typedef enum {
    LIGHTING_FRONT_STATUS_LIGHT_LEFT_INDICATOR_ON = 1,
    LIGHTING_FRONT_STATUS_LIGHT_LEFT_INDICATOR_OFF = 0,
} lighting_front_status_light_left_indicator_e;

typedef enum {
    LIGHTING_FRONT_STATUS_LIGHT_RIGHT_INDICATOR_ON = 1,
    LIGHTING_FRONT_STATUS_LIGHT_RIGHT_INDICATOR_OFF = 0,
} lighting_front_status_light_right_indicator_e;

typedef enum {
    LIGHTING_FRONT_STATUS_LIGHT_BPS_STROBE_ON = 1,
    LIGHTING_FRONT_STATUS_LIGHT_BPS_STROBE_OFF = 0,
} lighting_front_status_light_bps_strobe_e;

typedef enum {
    LIGHTING_FRONT_STATUS_LIGHT_BRAKELIGHT_ON = 1,
    LIGHTING_FRONT_STATUS_LIGHT_BRAKELIGHT_OFF = 0,
} lighting_front_status_light_brakelight_e;

typedef enum {
    LIGHTING_FRONT_STATUS_LIGHT_CUSTOMMODE_ON = 1,
    LIGHTING_FRONT_STATUS_LIGHT_CUSTOMMODE_OFF = 0,
} lighting_front_status_light_custommode_e;

typedef enum {
    LIGHTING_LEFT_STATUS_LIGHTING_BOARD_FAULT_WATCHDOG = 8,
    LIGHTING_LEFT_STATUS_LIGHTING_BOARD_FAULT_LIGHT_COMMAND_WATCHDOG = 7,
    LIGHTING_LEFT_STATUS_LIGHTING_BOARD_FAULT_LED1_OVERCURRENT = 6,
    LIGHTING_LEFT_STATUS_LIGHTING_BOARD_FAULT_LED0_OVERCURRENT = 5,
    LIGHTING_LEFT_STATUS_LIGHTING_BOARD_FAULT_ADDR_LED_OVERCURRENT = 4,
    LIGHTING_LEFT_STATUS_LIGHTING_BOARD_FAULT_LED1_UNDERCURRENT = 3,
    LIGHTING_LEFT_STATUS_LIGHTING_BOARD_FAULT_LED0_UNDERCURRENT = 2,
    LIGHTING_LEFT_STATUS_LIGHTING_BOARD_FAULT_ADDR_LED_UNDERCURRENT = 1,
    LIGHTING_LEFT_STATUS_LIGHTING_BOARD_FAULT_OK = 0,
} lighting_left_status_lighting_board_fault_e;

typedef enum {
    LIGHTING_LEFT_STATUS_LIGHT_HEADLIGHT_ON = 1,
    LIGHTING_LEFT_STATUS_LIGHT_HEADLIGHT_OFF = 0,
} lighting_left_status_light_headlight_e;

typedef enum {
    LIGHTING_LEFT_STATUS_LIGHT_LEFT_INDICATOR_ON = 1,
    LIGHTING_LEFT_STATUS_LIGHT_LEFT_INDICATOR_OFF = 0,
} lighting_left_status_light_left_indicator_e;

typedef enum {
    LIGHTING_LEFT_STATUS_LIGHT_RIGHT_INDICATOR_ON = 1,
    LIGHTING_LEFT_STATUS_LIGHT_RIGHT_INDICATOR_OFF = 0,
} lighting_left_status_light_right_indicator_e;

typedef enum {
    LIGHTING_LEFT_STATUS_LIGHT_BPS_STROBE_ON = 1,
    LIGHTING_LEFT_STATUS_LIGHT_BPS_STROBE_OFF = 0,
} lighting_left_status_light_bps_strobe_e;

typedef enum {
    LIGHTING_LEFT_STATUS_LIGHT_BRAKELIGHT_ON = 1,
    LIGHTING_LEFT_STATUS_LIGHT_BRAKELIGHT_OFF = 0,
} lighting_left_status_light_brakelight_e;

typedef enum {
    LIGHTING_LEFT_STATUS_LIGHT_CUSTOMMODE_ON = 1,
    LIGHTING_LEFT_STATUS_LIGHT_CUSTOMMODE_OFF = 0,
} lighting_left_status_light_custommode_e;

typedef enum {
    LIGHTING_REAR_STATUS_LIGHTING_BOARD_FAULT_WATCHDOG = 8,
    LIGHTING_REAR_STATUS_LIGHTING_BOARD_FAULT_LIGHT_COMMAND_WATCHDOG = 7,
    LIGHTING_REAR_STATUS_LIGHTING_BOARD_FAULT_LED1_OVERCURRENT = 6,
    LIGHTING_REAR_STATUS_LIGHTING_BOARD_FAULT_LED0_OVERCURRENT = 5,
    LIGHTING_REAR_STATUS_LIGHTING_BOARD_FAULT_ADDR_LED_OVERCURRENT = 4,
    LIGHTING_REAR_STATUS_LIGHTING_BOARD_FAULT_LED1_UNDERCURRENT = 3,
    LIGHTING_REAR_STATUS_LIGHTING_BOARD_FAULT_LED0_UNDERCURRENT = 2,
    LIGHTING_REAR_STATUS_LIGHTING_BOARD_FAULT_ADDR_LED_UNDERCURRENT = 1,
    LIGHTING_REAR_STATUS_LIGHTING_BOARD_FAULT_OK = 0,
} lighting_rear_status_lighting_board_fault_e;

typedef enum {
    LIGHTING_REAR_STATUS_LIGHT_HEADLIGHT_ON = 1,
    LIGHTING_REAR_STATUS_LIGHT_HEADLIGHT_OFF = 0,
} lighting_rear_status_light_headlight_e;

typedef enum {
    LIGHTING_REAR_STATUS_LIGHT_LEFT_INDICATOR_ON = 1,
    LIGHTING_REAR_STATUS_LIGHT_LEFT_INDICATOR_OFF = 0,
} lighting_rear_status_light_left_indicator_e;

typedef enum {
    LIGHTING_REAR_STATUS_LIGHT_RIGHT_INDICATOR_ON = 1,
    LIGHTING_REAR_STATUS_LIGHT_RIGHT_INDICATOR_OFF = 0,
} lighting_rear_status_light_right_indicator_e;

typedef enum {
    LIGHTING_REAR_STATUS_LIGHT_BPS_STROBE_ON = 1,
    LIGHTING_REAR_STATUS_LIGHT_BPS_STROBE_OFF = 0,
} lighting_rear_status_light_bps_strobe_e;

typedef enum {
    LIGHTING_REAR_STATUS_LIGHT_BRAKELIGHT_ON = 1,
    LIGHTING_REAR_STATUS_LIGHT_BRAKELIGHT_OFF = 0,
} lighting_rear_status_light_brakelight_e;

typedef enum {
    LIGHTING_REAR_STATUS_LIGHT_CUSTOMMODE_ON = 1,
    LIGHTING_REAR_STATUS_LIGHT_CUSTOMMODE_OFF = 0,
} lighting_rear_status_light_custommode_e;

typedef enum {
    LIGHTING_RIGHT_STATUS_LIGHTING_BOARD_FAULT_WATCHDOG = 8,
    LIGHTING_RIGHT_STATUS_LIGHTING_BOARD_FAULT_LIGHT_COMMAND_WATCHDOG = 7,
    LIGHTING_RIGHT_STATUS_LIGHTING_BOARD_FAULT_LED1_OVERCURRENT = 6,
    LIGHTING_RIGHT_STATUS_LIGHTING_BOARD_FAULT_LED0_OVERCURRENT = 5,
    LIGHTING_RIGHT_STATUS_LIGHTING_BOARD_FAULT_ADDR_LED_OVERCURRENT = 4,
    LIGHTING_RIGHT_STATUS_LIGHTING_BOARD_FAULT_LED1_UNDERCURRENT = 3,
    LIGHTING_RIGHT_STATUS_LIGHTING_BOARD_FAULT_LED0_UNDERCURRENT = 2,
    LIGHTING_RIGHT_STATUS_LIGHTING_BOARD_FAULT_ADDR_LED_UNDERCURRENT = 1,
    LIGHTING_RIGHT_STATUS_LIGHTING_BOARD_FAULT_OK = 0,
} lighting_right_status_lighting_board_fault_e;

typedef enum {
    LIGHTING_RIGHT_STATUS_LIGHT_HEADLIGHT_ON = 1,
    LIGHTING_RIGHT_STATUS_LIGHT_HEADLIGHT_OFF = 0,
} lighting_right_status_light_headlight_e;

typedef enum {
    LIGHTING_RIGHT_STATUS_LIGHT_LEFT_INDICATOR_ON = 1,
    LIGHTING_RIGHT_STATUS_LIGHT_LEFT_INDICATOR_OFF = 0,
} lighting_right_status_light_left_indicator_e;

typedef enum {
    LIGHTING_RIGHT_STATUS_LIGHT_RIGHT_INDICATOR_ON = 1,
    LIGHTING_RIGHT_STATUS_LIGHT_RIGHT_INDICATOR_OFF = 0,
} lighting_right_status_light_right_indicator_e;

typedef enum {
    LIGHTING_RIGHT_STATUS_LIGHT_BPS_STROBE_ON = 1,
    LIGHTING_RIGHT_STATUS_LIGHT_BPS_STROBE_OFF = 0,
} lighting_right_status_light_bps_strobe_e;

typedef enum {
    LIGHTING_RIGHT_STATUS_LIGHT_BRAKELIGHT_ON = 1,
    LIGHTING_RIGHT_STATUS_LIGHT_BRAKELIGHT_OFF = 0,
} lighting_right_status_light_brakelight_e;

typedef enum {
    LIGHTING_RIGHT_STATUS_LIGHT_CUSTOMMODE_ON = 1,
    LIGHTING_RIGHT_STATUS_LIGHT_CUSTOMMODE_OFF = 0,
} lighting_right_status_light_custommode_e;

typedef enum {
    LIGHTING_CANOPY_STATUS_LIGHTING_BOARD_FAULT_WATCHDOG = 8,
    LIGHTING_CANOPY_STATUS_LIGHTING_BOARD_FAULT_LIGHT_COMMAND_WATCHDOG = 7,
    LIGHTING_CANOPY_STATUS_LIGHTING_BOARD_FAULT_LED1_OVERCURRENT = 6,
    LIGHTING_CANOPY_STATUS_LIGHTING_BOARD_FAULT_LED0_OVERCURRENT = 5,
    LIGHTING_CANOPY_STATUS_LIGHTING_BOARD_FAULT_ADDR_LED_OVERCURRENT = 4,
    LIGHTING_CANOPY_STATUS_LIGHTING_BOARD_FAULT_LED1_UNDERCURRENT = 3,
    LIGHTING_CANOPY_STATUS_LIGHTING_BOARD_FAULT_LED0_UNDERCURRENT = 2,
    LIGHTING_CANOPY_STATUS_LIGHTING_BOARD_FAULT_ADDR_LED_UNDERCURRENT = 1,
    LIGHTING_CANOPY_STATUS_LIGHTING_BOARD_FAULT_OK = 0,
} lighting_canopy_status_lighting_board_fault_e;

typedef enum {
    LIGHTING_CANOPY_STATUS_LIGHT_HEADLIGHT_ON = 1,
    LIGHTING_CANOPY_STATUS_LIGHT_HEADLIGHT_OFF = 0,
} lighting_canopy_status_light_headlight_e;

typedef enum {
    LIGHTING_CANOPY_STATUS_LIGHT_LEFT_INDICATOR_ON = 1,
    LIGHTING_CANOPY_STATUS_LIGHT_LEFT_INDICATOR_OFF = 0,
} lighting_canopy_status_light_left_indicator_e;

typedef enum {
    LIGHTING_CANOPY_STATUS_LIGHT_RIGHT_INDICATOR_ON = 1,
    LIGHTING_CANOPY_STATUS_LIGHT_RIGHT_INDICATOR_OFF = 0,
} lighting_canopy_status_light_right_indicator_e;

typedef enum {
    LIGHTING_CANOPY_STATUS_LIGHT_BPS_STROBE_ON = 1,
    LIGHTING_CANOPY_STATUS_LIGHT_BPS_STROBE_OFF = 0,
} lighting_canopy_status_light_bps_strobe_e;

typedef enum {
    LIGHTING_CANOPY_STATUS_LIGHT_BRAKELIGHT_ON = 1,
    LIGHTING_CANOPY_STATUS_LIGHT_BRAKELIGHT_OFF = 0,
} lighting_canopy_status_light_brakelight_e;

typedef enum {
    LIGHTING_CANOPY_STATUS_LIGHT_CUSTOMMODE_ON = 1,
    LIGHTING_CANOPY_STATUS_LIGHT_CUSTOMMODE_OFF = 0,
} lighting_canopy_status_light_custommode_e;

/* ================= Message Structs ================= */

typedef struct {
    uint8_t Lighting_Set_Headlights;
    uint8_t Lighting_Set_Left_Indicator;
    uint8_t Lighting_Set_Right_Indicator;
    uint8_t Lighting_Blink_Sync;
    uint8_t Lighting_Set_Brake;
    uint8_t Lighting_Set_BPS_Strobe;
    uint8_t Lighting_Set_Custom_Mode;
} lighting_command_t;

typedef struct {
    uint8_t Lighting_Board_Fault;
    uint8_t Light_Headlight;
    uint8_t Light_Left_Indicator;
    uint8_t Light_Right_Indicator;
    uint8_t Light_BPS_Strobe;
    uint8_t Light_Brakelight;
    uint8_t Light_CustomMode;
    uint16_t Lighting_Addr_LED_Current;
    uint16_t Lighting_LED0_Current;
    uint16_t Lighting_LED1_Current;
} lighting_front_status_t;

typedef struct {
    uint8_t Lighting_Board_Fault;
    uint8_t Light_Headlight;
    uint8_t Light_Left_Indicator;
    uint8_t Light_Right_Indicator;
    uint8_t Light_BPS_Strobe;
    uint8_t Light_Brakelight;
    uint8_t Light_CustomMode;
    uint16_t Lighting_Addr_LED_Current;
    uint16_t Lighting_LED0_Current;
    uint16_t Lighting_LED1_Current;
} lighting_left_status_t;

typedef struct {
    uint8_t Lighting_Board_Fault;
    uint8_t Light_Headlight;
    uint8_t Light_Left_Indicator;
    uint8_t Light_Right_Indicator;
    uint8_t Light_BPS_Strobe;
    uint8_t Light_Brakelight;
    uint8_t Light_CustomMode;
    uint16_t Lighting_Addr_LED_Current;
    uint16_t Lighting_LED0_Current;
    uint16_t Lighting_LED1_Current;
} lighting_rear_status_t;

typedef struct {
    uint8_t Lighting_Board_Fault;
    uint8_t Light_Headlight;
    uint8_t Light_Left_Indicator;
    uint8_t Light_Right_Indicator;
    uint8_t Light_BPS_Strobe;
    uint8_t Light_Brakelight;
    uint8_t Light_CustomMode;
    uint16_t Lighting_Addr_LED_Current;
    uint16_t Lighting_LED0_Current;
    uint16_t Lighting_LED1_Current;
} lighting_right_status_t;

typedef struct {
    uint8_t Lighting_Board_Fault;
    uint8_t Light_Headlight;
    uint8_t Light_Left_Indicator;
    uint8_t Light_Right_Indicator;
    uint8_t Light_BPS_Strobe;
    uint8_t Light_Brakelight;
    uint8_t Light_CustomMode;
    uint16_t Lighting_Addr_LED_Current;
    uint16_t Lighting_LED0_Current;
    uint16_t Lighting_LED1_Current;
} lighting_canopy_status_t;

