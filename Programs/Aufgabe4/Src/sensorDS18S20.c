#include "global.h"
#include "lcd.h"
#include "onewire.h"
#include "errCodes.h"
#include "crc.h"
#include "sensorDS18S20.h"
#include "stm32f429xx.h"
#include "timing.h"
#include <stdint.h>
#include "math.h"
#include "ds18x20Common.h"

unsigned char crcPackDS18S20[8];
unsigned char tempDataDS18S20[9];

ThermometerDS18S20 DS18S20;

int sensorReadRom(){
    DS18S20.romID = 0;
    uint8_t byte = 0;
    int rc = OK;
    rc = oneWireReset(); 
    if (rc != OK) {
        return rc;
    }
    oneWireWriteByte(0x33);
    for (int i = 0; i < 8; i++) {
        rc = oneWireReadByte(&byte);
        if (rc!=OK) {
            return rc;
        }
        uint64_t temp = byte;
        DS18S20.romID = DS18S20.romID | (temp << (8*i));
        crcPackDS18S20[i] = (unsigned char) byte;
    }
    if (checkCRC(sizeof(crcPackDS18S20), crcPackDS18S20) == false) {
        return ERR_BIT_FLIPPED;
    }
    return rc;
} 

int sensorDS18S20ParseData() {
    DS18S20.raw.temperature = tempDataDS18S20[0] | tempDataDS18S20[1] << 8;
    DS18S20.raw.th = tempDataDS18S20[2];
    DS18S20.raw.tl = tempDataDS18S20[3];
    DS18S20.raw.config = tempDataDS18S20[4];
    DS18S20.raw.reserved = tempDataDS18S20[5];
    DS18S20.raw.countRemain = tempDataDS18S20[6];
    DS18S20.raw.countPerC = tempDataDS18S20[7];
    DS18S20.raw.crc = tempDataDS18S20[8];
    return OK;
}

int sensorDS18S20ReadData() {
    uint8_t byte = 0;
    int rc = OK;
    oneWireWriteByte(0xBE);
    for (int i = 0; i < 9; i++) {
        rc = oneWireReadByte(&byte);
        if (rc!=OK) {
            return rc;
        }
        tempDataDS18S20[i] = (unsigned char) byte; 
    }

    if (checkCRC(sizeof(tempDataDS18S20), tempDataDS18S20) == false) {
        return ERR_BIT_FLIPPED;
    }
    rc = sensorDS18S20ParseData();
    return rc;
}

int sensorDS18S20GetTemperature() {
    
    int rc = OK;

   rc = sensorSelect(DS18S20.romID);
   if (rc!=OK) {
    return rc;
   }
   oneWireWriteByte(0x44);

   sensorFullThrottle();

   rc = sensorSelect(DS18S20.romID);
   if (rc!=OK) {
    return rc;
   }

   rc = sensorDS18S20ReadData();
   if (rc!=OK) {
    return rc;
   }

   rc = tempCalcCelciusDS18S20(&DS18S20);
   lcdPrintInt(DS18S20.valueCelcius);
   return rc;
   
}