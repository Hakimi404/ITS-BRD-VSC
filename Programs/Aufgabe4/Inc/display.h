#ifndef DISPLAY_H
#define DISPLAY_H

#include "sensorDS18B20.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

void initDisplay();
void drawAllInfo(char *sensorName, ThermometerDS18B20 *sensors, int sensorCount);
void drawInfo(char* sensorName, char *ID, int sensorNumber);
void writeToError(char* ErrorCode, char* msg);
void clearError();
void clearContents(char blancCh, int contentsStart_y, const char* RowContents, size_t rowContentsSize);
void drawTemperatureDS18B20(int sensorCount, bool *tempReaded);
void convertAllValuesToDisplay(ThermometerDS18B20 *data, int sensorCount);
#endif