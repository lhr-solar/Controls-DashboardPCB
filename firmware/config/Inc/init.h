#include "Status_LEDs.h"
#include "Switches.h"
#include "stm32xx_hal.h"
#include "Horn.h"

void GPIO_Init(); // Initializes all GPIO pins/ports
void gpioPin_Init(GPIO_Pin pin, uint32_t mode); // Initializes a GPIO pin with the specified mode (input/output/interrupt)
void gpioEXTI_Init(GPIO_Pin pin); // Initializes a GPIO pin as an external interrupt source