#include "math.h"


int tempCalcCelciusDS18S20(ThermometerDS18S20 *sensorData)
{
    /*
     * Formel aus dem Datenblatt des DS18S20:
     *
     * T = Temperatur - 0.25 + (CountPerC - CountRemain) / CountPerC
     */
    sensorData->valueCelcius =
        (float)(sensorData->raw.temperature)
        - 0.25f
        + (float)(sensorData->raw.countPerC - sensorData->raw.countRemain)
          / sensorData->raw.countPerC;

    return OK;
}

//Berechnet die Temperatur für alle angeschlossenen DS18B20 Sensoren Jeder Rohwert wird mit 0.0625 multipliziert, da ein Bit genau  0.0625 °C entspricht.
int tempCalcCelciusDS18B20(ThermometerDS18B20 *sensorArray, int sensorCount)
{
    for (int i = 0; i < sensorCount; i++) {

        /*
         * DS18B20:
         * 1 LSB = 0.0625 °C
         */
        sensorArray[i].valueCelcius =
            (float)(sensorArray[i].raw.temperature) * 0.0625f;
    }

    return OK;
}
