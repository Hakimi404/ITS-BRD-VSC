#include "stm32f429xx.h"
#include "timing.h"
#include <stdint.h>
#include "errCodes.h"

int wait(uint32_t duration) {
    duration = duration * 90;
    uint32_t startTime = TIM2->CNT;
    while ((TIM2->CNT - startTime) < duration) {
    }
    return OK;
}