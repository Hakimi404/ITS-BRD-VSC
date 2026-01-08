#ifndef MATH_H
#define MATH_H

#include "sensorDS18B20.h"
#include "sensorDS18S20.h"
#include "errCodes.h"

int tempCalcCelciusDS18B20(ThermometerDS18B20 *data, int deviceCount);
int tempCalcCelciusDS18S20(ThermometerDS18S20 *data);
#endif