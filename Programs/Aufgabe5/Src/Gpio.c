#include "gpio.h"
#include "input.h"
#include "stm32f4xx_hal.h"


int readGPIOPin(GPIO_TypeDef *GPIOx, int pin)
{
    return (GPIOx->IDR & (1 << pin)) ? 1 : 0;
}

// Configure PF0/PF1 as EXTI interrupts for encoder inputs
void initEncoderInterrupts(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // PF0 / PF1 as interrupt inputs (both edges)
    GPIO_InitStruct.Pin  = (uint16_t)((1U << S0) | (1U << S1));
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(BUTTON_PORT, &GPIO_InitStruct);

    // Enable EXTI interrupts
    HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI0_IRQn);

    HAL_NVIC_SetPriority(EXTI1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI1_IRQn);
}

// HAL callback -> call shared encoder ISR
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == (1U << S0) || GPIO_Pin == (1U << S1)) {
        encoder_isr();
    }
}

//IRQ HANDLERS (MANDATORY for EXTI to work)
void EXTI0_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
}

void EXTI1_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_1);
}
