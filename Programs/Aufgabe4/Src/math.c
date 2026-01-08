#include "math.h"


int tempCalcCelciusDS18S20(ThermometerDS18S20 *data) {
    (*data).valueCelcius = (float)((*data).raw.temperature) - 0.25f + ((float)((*data).raw.countPerC - (*data).raw.countRemain)/(*data).raw.countPerC);
    return OK;
}

int tempCalcCelciusDS18B20(ThermometerDS18B20* data, int deviceCount) {
    for (int i = 0; i < deviceCount; i++) {
        data[i].valueCelcius = (float)data[i].raw.temperature * 0.0625;
    }
    return OK;
}   