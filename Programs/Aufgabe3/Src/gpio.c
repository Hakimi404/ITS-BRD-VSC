#include "gpio.h"

int readGPIOPin(GPIO_TypeDef* GPIOx, int pin)
{
    return (GPIOx->IDR & (1 << pin)) != 0;
}
