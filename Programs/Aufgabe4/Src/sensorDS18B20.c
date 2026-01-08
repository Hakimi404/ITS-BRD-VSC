#include "sensorDS18B20.h"
#include "crc.h"
#include "display.h"
#include "ds18x20Common.h"
#include "errCodes.h"
#include "global.h"
#include "lcd.h"
#include "math.h"
#include "onewire.h"
#include "stm32f429xx.h"
#include "timing.h"
#include <stdint.h>
#include <stdlib.h>

unsigned char crcPackDS18B20[8];
unsigned char tempDataDS18B20[9];
uint64_t deviceIDs[MAX_SUPPORTET_DEVICES];
int devicesCount = 0;
bool tempReaded = false;

ThermometerDS18B20 DS18B20[MAX_SUPPORTET_DEVICES];
int sensorDS18B20SearchRom() {
  int rc = OK;
  rc = scanOneWireBus(deviceIDs, &devicesCount);
  if (rc != OK) {
    return rc;
  }
  for (int i = 0; i < devicesCount; i++) {
    DS18B20[i].romID = deviceIDs[i];
    drawInfo("DS18B20", DS18B20[i].romID, i + 1);
  }
  return rc;
}

int sensorDS18B20ParseData(int deviceNumber) {
  DS18B20[deviceNumber].raw.temperature =
      tempDataDS18B20[0] | tempDataDS18B20[1] << 8;
  DS18B20[deviceNumber].raw.th = tempDataDS18B20[2];
  DS18B20[deviceNumber].raw.tl = tempDataDS18B20[3];
  DS18B20[deviceNumber].raw.config = tempDataDS18B20[4];
  DS18B20[deviceNumber].raw.reserved1 = tempDataDS18B20[5];
  DS18B20[deviceNumber].raw.reserved2 = tempDataDS18B20[6];
  DS18B20[deviceNumber].raw.reserved3 = tempDataDS18B20[7];
  DS18B20[deviceNumber].raw.crc = tempDataDS18B20[8];
  return OK;
}

int sensorDS18B20ReadData(int deviceNumber) {
  uint8_t byte = 0;
  int rc = OK;
  oneWireWriteByte(0xBE);
  for (int i = 0; i < 9; i++) {
    rc = oneWireReadByte(&byte);
    if (rc != OK) {
      return rc;
    }
    tempDataDS18B20[i] = (unsigned char)byte;
  }

  if (checkCRC(sizeof(tempDataDS18B20), tempDataDS18B20) == false) {
    return ERR_BIT_FLIPPED;
  }
  rc = sensorDS18B20ParseData(deviceNumber);
  return rc;
}

int sensorDS18B20GetTemperature() {
  int rc = OK;
  if (!tempReaded) {
    oneWireReset();
    oneWireWriteByte(0xCC);
    oneWireWriteByte(0x44);
    sensorFullThrottle();
    for (int i = 0; i < devicesCount; i++) {

      rc = sensorSelect(DS18B20[i].romID);
      if (rc != OK) {
        return rc;
      }

      rc = sensorDS18B20ReadData(i);
      if (rc != OK) {
        return rc;
      }
    }
    rc = tempCalcCelciusDS18B20(DS18B20, devicesCount); 
    if (rc!=OK) {
        return rc;
    }
    convertAllValuesToDisplay(DS18B20, devicesCount);
    tempReaded = true;
  } if (tempReaded) {
    drawTemperatureDS18B20(devicesCount, &tempReaded);
  }
  return rc;
}