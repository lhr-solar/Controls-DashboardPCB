#include "pinMode.h"

#ifndef STATUS_LEDS_H
#define STATUS_LEDS_H

//Port A - out
extern const GPIO_Pin PH_CAN_RX_LED;
extern const GPIO_Pin BPS_Fault_LED;

//Port B - out
extern const GPIO_Pin CarCAN_RX_LED;
extern const GPIO_Pin CarCAN_TX_LED;
extern const GPIO_Pin PH_CAN_TX_LED;
extern const GPIO_Pin X_LED2;

//Port C - out
extern const GPIO_Pin Controls_HB_LED;
extern const GPIO_Pin LSOM_HB;
extern const GPIO_Pin AKSHAY_LED;


// #define Indicator_PWM_Pin    GPIO_PIN_15
// #define Indicator_PWM_Port   GPIOB

void LED_GPIO_Init();

//void set_LED(GPIO_Pin LED, GPIO_PinState state);
void set_LED(GPIO_Pin LED, GPIO_PinState state);
void toggle_LED(GPIO_Pin LED);
void flash_them();
void RainbowLED();

#endif