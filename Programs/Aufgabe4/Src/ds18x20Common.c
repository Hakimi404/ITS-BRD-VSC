#include "ds18x20Common.h"
#include "crc.h"
#include "display.h"
#include <locale.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "global.h"

#define PUSH_PULL_DURATION 750000

uint64_t oldRegistration = 0;
uint64_t registration = 0;
int lastDiscrepancy = -1;
int newDiscrepancy = -1;
bool lastDeviceFlag = 0;
bool checkImProzess = false; 
unsigned char crcPack[8];

void sensorFullThrottle() {
    GPIOD->OTYPER &= ~PD0_MASK;
    GPIOD->BSRR = PD0_MASK;
    wait(PUSH_PULL_DURATION);
    GPIOD->OTYPER |= PD0_MASK;
}

int sensorSelect(uint64_t targetID) {
    oneWireReset();
    int rc = OK;
    oneWireWriteByte(0x55);
    for (int i = 0; i < 8; i++) {
        rc = oneWireWriteByte((uint8_t)((targetID >> (8*i))));
        if (rc!=OK) {
            return rc;
        }
    }
    return OK;

}
int prepareForCrcCheck() {
    int rc = OK;
    for (int i = 0; i < 8; i++) {
        crcPack[i] = registration >> 8 * i;
    }
    rc = checkCRC(8, crcPack);
    return rc;
}

int scanOneWireBus(uint64_t *deviceIDs, int* devicesCount){
    int rc = OK;
    lastDeviceFlag = false;
    oldRegistration = 0;
    registration = 0;
    lastDiscrepancy = -1;
    newDiscrepancy = -1;
    lastDeviceFlag = 0;
    do {
        rc = oneWireReset();
        if (rc != OK) {
            return rc;
        }
        rc = oneWireWriteByte(0xF0);
        if (rc!=OK) {
            return rc;
        }
        int bit1 = 0;
        int bit2 = 0;

        oldRegistration = registration;
        registration = 0;
        newDiscrepancy = -1;
        uint64_t direction = 0;
            for (int i = 0; i < 64; i++) {
                oneWireReadBit(&bit1);
                oneWireReadBit(&bit2);
                if (bit1 == bit2 && bit1 == 1) {
                    return ERR_NO_SENSOR;
                }
                if (bit1 != bit2) { 
                    direction = bit1;
                } 
                else {
                    if (i < lastDiscrepancy) {
                        direction = oldRegistration >> i & 0x01U;
                    }
                    else if (i == lastDiscrepancy) {
                        direction = 1;
                    }
                    else if (i > lastDiscrepancy && bit1 == bit2 && bit1 == 0) {
                        direction = 0;
                    }
                    if (direction == 0) {
                        newDiscrepancy = i;
                    }
                }
                
                oneWireWriteBit(direction);
                registration |= direction << i;
            }

            lastDiscrepancy = newDiscrepancy;

            if ((*devicesCount)< MAX_SUPPORTET_DEVICES) {
                deviceIDs[(*devicesCount)] = registration;
                (*devicesCount)++;
            }

            if (lastDiscrepancy == -1) {
                lastDeviceFlag = true;
            }
           
            if (rc != OK) {
                return rc;
            }
    }

    while (!lastDeviceFlag);
    
    return OK;
}