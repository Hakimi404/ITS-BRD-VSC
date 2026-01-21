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
#define PORTF_EXTICRMASK 0x05U
#define EXTIPORT_SIZE 0x4U
#define PIN_0 0x00U
#define PIN_1 0x01U
#define MODER_INPUT 0x00U
#define MODER_BITPERPIN 0x2U
#define PUPDR_PULLUP 0x1U
#define PUPDR_BITPERPIN 0x2U
#define TWOBITCLEAR_MASK 0x3U

int readGPIOPin(GPIO_TypeDef *GPIOx, int pin);

// Configure encoder inputs (S0/S1 = PF0/PF1) as EXTI interrupts
void initEncoderInterrupts(void);

#endif
