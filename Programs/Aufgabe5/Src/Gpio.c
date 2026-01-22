    #include "gpio.h"
    #include "input.h"
    #include "stm32f429xx.h"
    #include "stm32f4xx_hal.h"
    #include "stm32f4xx_hal_cortex.h"
    #include "stm32f4xx_hal_gpio.h"

    int readGPIOPin(GPIO_TypeDef *GPIOx, int pin)
    {
        return (GPIOx->IDR & (1 << pin)) ? 1 : 0;
    }

    // Configure PF0/PF1 as EXTI interrupts for encoder inputs
    void initEncoderInterrupts(void)
    {   
        GPIO_InitTypeDef GPIO_InitStruct = {0};

HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOFEN;
        RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

        GPIOE->MODER &= ~(TWOBITCLEAR_MASK << (PIN_0 * MODER_BITPERPIN));
        GPIOE->MODER |= (0x01 << (PIN_0 * MODER_BITPERPIN));

        // PF0 u. PF1 Input
        GPIOF->MODER &= ~(TWOBITCLEAR_MASK << (PIN_0 * MODER_BITPERPIN)); 
        GPIOF->MODER &= ~(TWOBITCLEAR_MASK << (PIN_1 * MODER_BITPERPIN));
        GPIOF->MODER |= (MODER_INPUT << (PIN_0 * MODER_BITPERPIN)); 
        GPIOF->MODER |= (MODER_INPUT << (PIN_1 * MODER_BITPERPIN));

        // PF0 u. PF1 Pullup
        GPIOF->PUPDR &= ~(TWOBITCLEAR_MASK << (PIN_0 * PUPDR_BITPERPIN));
        GPIOF->PUPDR &= ~(TWOBITCLEAR_MASK << (PIN_1 * PUPDR_BITPERPIN));
        GPIOF->PUPDR |= (PUPDR_PULLUP << (PIN_0 * PUPDR_BITPERPIN));
        GPIOF->PUPDR |= (PUPDR_PULLUP << (PIN_1 * PUPDR_BITPERPIN));

        // Alte Werte entfernen und gew. Pisn einstellen
        SYSCFG->EXTICR[0] &= ~(0x0F << (EXTIPORT_SIZE * PIN_0));
        SYSCFG->EXTICR[0] &= ~(0x0F << (EXTIPORT_SIZE * PIN_1));
        SYSCFG->EXTICR[0] |= PORTF_EXTICRMASK << (EXTIPORT_SIZE * PIN_0);
        SYSCFG->EXTICR[0] |= PORTF_EXTICRMASK << (EXTIPORT_SIZE * PIN_1);

        // jegliche Veränderung triggert Interrupt
        EXTI->RTSR |= (1 << PIN_0);
        EXTI->FTSR |= (1 << PIN_0);
        EXTI->RTSR |= (1 << PIN_1);
        EXTI->FTSR |= (1 << PIN_1);

        // Konfigurierte Interrupts entmaskieren 
        EXTI->IMR |= (1 << PIN_0);  
        EXTI->IMR |= (1 << PIN_1);

        // Priorität einstellen und aktivieren
        NVIC_SetPriority(EXTI0_IRQn, 0);
        NVIC_SetPriority(EXTI1_IRQn, 0);
        NVIC_EnableIRQ(EXTI0_IRQn);
        NVIC_EnableIRQ(EXTI1_IRQn);
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
