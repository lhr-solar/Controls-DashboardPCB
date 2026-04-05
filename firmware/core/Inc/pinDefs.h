#pragma once

#include "stm32xx_hal.h"

/* -------- Output LEDs - CAN -------- */
#define PH_CAN_RX_LED_PORT      GPIOA
#define PH_CAN_RX_LED_PIN       GPIO_PIN_10
#define CAR_CAN_RX_LED_PORT     GPIOB
#define CAR_CAN_RX_LED_PIN      GPIO_PIN_3
#define CAR_CAN_TX_LED_PORT     GPIOB
#define CAR_CAN_TX_LED_PIN      GPIO_PIN_5
#define PH_CAN_TX_LED_PORT      GPIOB
#define PH_CAN_TX_LED_PIN       GPIO_PIN_4

/* -------- Output LEDs - Status -------- */
#define INDICATION_PWN_PORT 	GPIOB
#define INDICATION_PWM_PIN		GPIO_PIN15
#define X_LED2_PORT             GPIOB
#define X_LED2_PIN              GPIO_PIN_14
#define CONTROLS_HB_LED_PORT    GPIOB
#define CONTROLS_HB_LED_PIN     GPIO_PIN_10
#define AKSHAY_LED_PORT         GPIOB
#define AKSHAY_LED_PIN          GPIO_PIN_11
#define BPS_FAULT_LED_PORT      GPIOC
#define BPS_FAULT_LED_PIN       GPIO_PIN_1
#define LSOM_HB_PORT            GPIOC
#define LSOM_HB_PIN             GPIO_PIN_3

/* -------- Output - GPIO -------- */
#define HORN_OUT_PORT           GPIOB
#define HORN_OUT_PIN            GPIO_PIN_1

/* -------- Input - Ignition -------- */
#define IGN_OFF_PORT            GPIOA
#define IGN_OFF_PIN             GPIO_PIN_2
#define IGN_MTR_PORT            GPIOA
#define IGN_MTR_PIN             GPIO_PIN_1
#define IGN_ARR_PORT            GPIOB
#define IGN_ARR_PIN             GPIO_PIN_0

/* -------- Input - Switches -------- */
#define FWD_SW_PORT             GPIOA
#define FWD_SW_PIN              GPIO_PIN_0
#define REV_SW_PORT             GPIOC
#define REV_SW_PIN              GPIO_PIN_7
#define NEUTRAL_GEAR_PORT       GPIOC
#define NEUTRAL_GEAR_PIN        GPIO_PIN_0
#define PTT_PORT                GPIOB
#define PTT_PIN                 GPIO_PIN_2
#define HORN_PORT               GPIOD
#define HORN_PIN                GPIO_PIN_2

/* -------- Input - Cruise -------- */
#define CRUISE_ENABLE_PORT      GPIOC
#define CRUISE_ENABLE_PIN       GPIO_PIN_6
#define CRUISE_SET_PORT         GPIOC
#define CRUISE_SET_PIN          GPIO_PIN_9

/* -------- Input - Regen -------- */
#define REGEN_ENABLE_PORT       GPIOC
#define REGEN_ENABLE_PIN        GPIO_PIN_2
#define REGEN_ACTIVE_PORT       GPIOC
#define REGEN_ACTIVE_PIN        GPIO_PIN_12

/* -------- Input - Blinkers -------- */
#define RIGHT_BLINKER_PORT      GPIOC
#define RIGHT_BLINKER_PIN       GPIO_PIN_4
#define LEFT_BLINKER_PORT       GPIOC
#define LEFT_BLINKER_PIN        GPIO_PIN_5
#define HAZARD_PORT             GPIOC
#define HAZARD_PIN              GPIO_PIN_8