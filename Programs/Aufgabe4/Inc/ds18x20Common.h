#ifndef DS18X20COMMON_H
#define DS18X20COMMON_H

#include "stm32f429xx.h"
#include "global.h"
#include "timing.h"
#include "errCodes.h"
#include "onewire.h"

void sensorFullThrottle();
int sensorSelect(uint64_t targetID);
int scanOneWireBus(uint64_t *deviceIDs, int *devicesCount);

#endif