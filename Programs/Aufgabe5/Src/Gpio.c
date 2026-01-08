#include "gpio.h"

int readGPIOPin(GPIO_TypeDef *GPIOx, int pin) { //Liest den Zustand (0 oder 1) eines Pins
    return (GPIOx->IDR & (1 << pin)) ? 1 : 0; //Pin maskieren und 0 oder 1 zurückgeben
}
