#include "pinMode.h"

#ifndef HORN_H
#define HORN_H

extern const GPIO_Pin Horn_OUT;

void Horn_GPIO_Init();
void set_Horn(SwitchState state);

#endif