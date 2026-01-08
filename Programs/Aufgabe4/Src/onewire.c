#include "onewire.h"
#include "stm32f429xx.h"
#include "timing.h"
#include "errCodes.h"
#include "global.h"


int oneWireReset() {
    GPIOD->OTYPER &= ~(PD1_MASK); //open drain 
    GPIOD->BSRR = PD1_MASK; //open drain high 

    GPIOD->OTYPER |= PD0_MASK; //push pull 
    GPIOD->BSRR = PD0_MASK << 16; // push pull port auf low
    wait (480);
    GPIOD->BSRR = PD0_MASK; //push pull auf high (bus low)
    wait (70);
    if ((GPIOD->IDR & PD0_MASK) != DEFAULT) {
        return ERR_NO_SENSOR;
    }
    wait(410);
    return OK;
}

int oneWireWriteBit(int bit) {
   switch (bit)
   {
   case 0x00:
   GPIOD->BSRR = PD0_MASK << 16; //push pull auf high (bus low)
   wait(60);
   GPIOD->BSRR = PD0_MASK; //push pull auf low (bus high)
   wait(10);
    break;
   case 0x01:
   GPIOD->BSRR = PD0_MASK << 16; //push pull auf high (bus low)
   wait(6);
   GPIOD->BSRR = PD0_MASK; //push pull auf low (bus high)
   wait(64);
   break;
   default:
   return ERR_UNKNOWN_BIT;
    break;
   }
   return OK;
}

int oneWireReadBit(int *bit) {
   GPIOD -> BSRR = PD0_MASK << 16; //push pull auf high (bus low)
   wait(6);
   GPIOD->BSRR = PD0_MASK; //push pull auf low (bus high)
   wait (9);
   if((GPIOD->IDR & PD0_MASK) != DEFAULT) { 
    (*bit) = 0x01U;
   }
   else {
    (*bit) = DEFAULT;
   }
   wait(55);
   return OK;
}

int oneWireWriteByte(uint8_t byte) {
    int rc = OK;
    for (int i = 0; i < 8; i++) {
        rc = oneWireWriteBit((byte >> i) & BASE); 
        if (rc!=OK) {
            return rc;
        }
    }
    return rc;
}

int oneWireReadByte(uint8_t *byte) {
   (*byte) = DEFAULT;
   int rc = OK;
   int bit = 0x00;
   for (int i = 0; i < 8; i++) {
        rc = oneWireReadBit(&bit);
        if (rc != OK) {
            return rc;
        }
       (*byte) |= (bit << i); 
    }
   return rc;
}