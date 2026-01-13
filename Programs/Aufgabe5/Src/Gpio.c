#include "gpio.h"
#include "stm32f4xx_hal.h"
#include "input.h"

/* =========================================================
 * GPIO READ (unchanged, still used everywhere)
 * ========================================================= */
int readGPIOPin(GPIO_TypeDef *GPIOx, int pin)
{
    return (GPIOx->IDR & (1 << pin)) ? 1 : 0;
}

/* =========================================================
 * ENCODER GPIO INTERRUPT INITIALIZATION (NEW)
 * ========================================================= */
void initEncoderInterrupts(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* AUX0 & AUX1 as interrupt inputs */
    GPIO_InitStruct.Pin  = (1 << AUX0) | (1 << AUX1);
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(AUX_PORT, &GPIO_InitStruct);

    /* Enable EXTI interrupts */
    HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI0_IRQn);

    HAL_NVIC_SetPriority(EXTI1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI1_IRQn);
}

/* =========================================================
 * HAL CALLBACK → ENCODER ISR
 * ========================================================= */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    /* Any AUX pin triggers the same ISR logic */
    if (GPIO_Pin == (1 << AUX0) || GPIO_Pin == (1 << AUX1)) {
        encoder_isr();
    }
}
