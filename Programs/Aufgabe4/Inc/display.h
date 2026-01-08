#ifndef DISPLAY_H
#define DISPLAY_H

#include "sensorDS18B20.h"
#include <stdbool.h>
#include <stdint.h>
void initDisplay();
void drawInfo(char* sensorName, uint64_t ID, int sensorNumber);
void drawTemperatureDS18B20(int sensorCount, bool *tempReaded);
void convertAllValuesToDisplay(ThermometerDS18B20 *data, int sensorCount);
#endif