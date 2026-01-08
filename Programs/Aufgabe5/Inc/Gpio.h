#ifndef GPIO_H
#define GPIO_H

#include "stm32f4xx_hal.h"

#define BUTTON_PORT GPIOF
#define S0 0
#define S1 1
#define S6 6
#define S7 7
#define D21 (1 << 5)
#define D22 (1 << 6)
#define D23 (1 << 7)

int readGPIOPin(GPIO_TypeDef *GPIOx, int pin);

#endif
