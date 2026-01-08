#include "sensorDS18S20.h"
#include "onewire.h"
#include "ds18x20Common.h"
#include "crc.h"
#include "math.h"
#include "lcd.h"
#include "errCodes.h"
#include "global.h"
#include "timing.h"

#include <stdint.h>

//Puffer für CRC- und Scratchpad-Daten
static unsigned char scratchpadData[9];
static unsigned char crcBuffer[8];

//Datenstruktur für den DS18S20 
static ThermometerDS18S20 sensor;

//Liest die ROM-ID des DS18S20 Sensors.Diese Funktion wird nur verwendet, wenn genau ein Sensor vorhanden ist.
int sensorReadRom(void)
{
    uint8_t receivedByte = 0;
    int result = OK;

    sensor.romID = 0;

    //1-Wire-Bus zurücksetzen
    result = oneWireReset();
    if (result != OK) {
        return result;
    }

    //READ ROM Befehl (nur bei EINEM Sensor erlaubt)
    oneWireWriteByte(0x33);

    //64-Bit ROM-ID auslesen
    for (int byteIndex = 0; byteIndex < 8; byteIndex++) {

        result = oneWireReadByte(&receivedByte);
        if (result != OK) {
            return result;
        }

        sensor.romID |= ((uint64_t)receivedByte << (8 * byteIndex));
        crcBuffer[byteIndex] = receivedByte;
    }

    //CRC der ROM-ID prüfen
    if (!checkCRC(sizeof(crcBuffer), crcBuffer)) {
        return ERR_BIT_FLIPPED;
    }

    return OK;
}

//Wandelt die Scratchpad-Daten in die interne Datenstruktur um
static int sensorParseScratchpad(void)
{
    sensor.raw.temperature  = scratchpadData[0] | (scratchpadData[1] << 8);
    sensor.raw.th           = scratchpadData[2];
    sensor.raw.tl           = scratchpadData[3];
    sensor.raw.config       = scratchpadData[4];
    sensor.raw.reserved     = scratchpadData[5];
    sensor.raw.countRemain  = scratchpadData[6];
    sensor.raw.countPerC    = scratchpadData[7];
    sensor.raw.crc          = scratchpadData[8];

    return OK;
}

//Liest die Scratchpad-Daten des Sensors aus.
static int sensorReadScratchpad(void)
{
    uint8_t receivedByte = 0;
    int result = OK;

    //READ SCRATCHPAD Befehl 
    oneWireWriteByte(0xBE);

    //9 Bytes Scratchpad lesen
    for (int i = 0; i < 9; i++) {

        result = oneWireReadByte(&receivedByte);
        if (result != OK) {
            return result;
        }

        scratchpadData[i] = receivedByte;
    }

    //CRC prüfen
    if (!checkCRC(sizeof(scratchpadData), scratchpadData)) {
        return ERR_BIT_FLIPPED;
    }

    return sensorParseScratchpad();
}

//Führt eine komplette Temperaturmessung durch und gibt die Temperatur auf dem LCD aus.
int sensorDS18S20GetTemperature(void)
{
    int result = OK;

    //Sensor auswählen
    result = sensorSelect(sensor.romID);
    if (result != OK) {
        return result;
    }

    //Temperaturmessung starten 
    oneWireWriteByte(0x44);

    //Starken Pull-Up aktivieren
    sensorFullThrottle();

    //Sensor erneut auswählen
    result = sensorSelect(sensor.romID);
    if (result != OK) {
        return result;
    }

    //Scratchpad lesen
    result = sensorReadScratchpad();
    if (result != OK) {
        return result;
    }

    // Rohdaten in Grad Celsius umrechnen
    result = tempCalcCelciusDS18S20(&sensor);
    if (result != OK) {
        return result;
    }

    //Temperatur anzeigen
    lcdPrintInt(sensor.valueCelcius);

    return OK;
}
