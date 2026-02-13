#include "pinMode.h"

#ifndef SWITCHES_H
#define SWITCHES_H

// Port A - input
extern const GPIO_Pin FWD_SW;
extern const GPIO_Pin Ign_OFF;
extern const GPIO_Pin Ign_MTR;

// Port B - input
extern const GPIO_Pin Ign_ARR;
extern const GPIO_Pin PTT;

// Port C - input
extern const GPIO_Pin Neutral_Gear;
extern const GPIO_Pin Rev_SW;

extern const GPIO_Pin Regen_Enable;
extern const GPIO_Pin Right_Blinker;
extern const GPIO_Pin Left_Blinker;
extern const GPIO_Pin Cruise_Enable;
extern const GPIO_Pin Cruise_Set;
extern const GPIO_Pin Regen_Active;
extern const GPIO_Pin Hazard;

// Port D - input
extern const GPIO_Pin Horn;


//Input State Bits Map
#define FWD_SW_BIT          (1 << 0)
#define IGN_OFF_BIT         (1 << 1)
#define IGN_MTR_BIT         (1 << 2)
#define IGN_ARR_BIT         (1 << 3)
#define NEUTRAL_GEAR_BIT    (1 << 4)
#define REV_SW_BIT          (1 << 5)


typedef enum {
    ON = GPIO_PIN_RESET,
    OFF = GPIO_PIN_SET
} SwitchState;

SwitchState get_switch_state(GPIO_Pin sw);



#endif