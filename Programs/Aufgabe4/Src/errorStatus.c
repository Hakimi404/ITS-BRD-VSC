#include "errorStatus.h"
#include "stm32f429xx.h"
#include "timing.h"
#include "errCodes.h"

#define ERROR_LED_BASE (1)

void indicateError(int rc) {
    int led = 0;
    switch (rc)
    {
    case ERR_UNKNOWN_BIT:
        led = ERROR_LED_BASE << 1;
        break;
    case ERR_BIT_FLIPPED:
        led = ERROR_LED_BASE << 2;
        break;
    
    case ERR_NO_SENSOR:
        led = ERROR_LED_BASE << 7;
        break;
    default:
        break;
    }
    while (1) {
        wait(1000000);
        GPIOE->BSRR = led;
        wait(1000000);
        GPIOE->BSRR = led << 16;
    }
}