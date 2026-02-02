#include "pinMode.h"

#ifndef STATUS_LEDS_H
#define STATUS_LEDS_H

//Port A - out
static const GPIO_Pin PH_CAN_RX       = { GPIOA, GPIO_PIN_15 };
static const GPIO_Pin BPS_Fault_LED   = { GPIOA, GPIO_PIN_6  };

//Port B - out
static const GPIO_Pin CarCan_RX       = { GPIOB, GPIO_PIN_3  };
static const GPIO_Pin CarCan_TX       = { GPIOB, GPIO_PIN_5  };
static const GPIO_Pin PH_CAN_TX       = { GPIOB, GPIO_PIN_4  };
static const GPIO_Pin X_LED2          = { GPIOB, GPIO_PIN_14 };

//Port C - out
static const GPIO_Pin Controls_HB_LED = { GPIOC, GPIO_PIN_11 };
static const GPIO_Pin LSOM_HB         = { GPIOC, GPIO_PIN_3  };
static const GPIO_Pin AKSHAY_LED      = { GPIOC, GPIO_PIN_10 };


// #define Indicator_PWM_Pin    GPIO_PIN_15
// #define Indicator_PWM_Port   GPIOB


//void set_LED(GPIO_Pin LED, GPIO_PinState state);
void set_LED(GPIO_Pin LED, GPIO_PinState state);
void toggle_LED(GPIO_Pin LED);
void flash_them();

#endif