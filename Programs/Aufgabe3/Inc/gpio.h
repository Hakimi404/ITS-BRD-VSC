#ifndef GPIO_H
#define GPIO_H

#include "stm32f4xx_hal.h"

int readGPIOPin(GPIO_TypeDef* GPIOx, int pin);

#endif
