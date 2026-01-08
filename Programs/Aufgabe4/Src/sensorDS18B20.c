#include "sensorDS18B20.h"
#include "crc.h"
#include "display.h"
#include "ds18x20Common.h"
#include "errCodes.h"
#include "global.h"
#include "lcd.h"
#include "math.h"
#include "onewire.h"
#include "timing.h"

#include <stdint.h>
#include <stdbool.h>


//Zwischenspeicher für CRC und Scratchpad
static unsigned char scratchpadData[9];

//Gefundene Sensor-IDs
static uint64_t foundDeviceIds[MAX_SUPPORTET_DEVICES];
static int numberOfDevices = 0;

//Status: Wurde Temperatur bereits gelesen?
static bool temperatureReadDone = false;

//Datenstruktur für alle DS18B20 Sensoren
static ThermometerDS18B20 sensors[MAX_SUPPORTET_DEVICES];

//Sucht alle DS18B20 Sensoren auf dem 1-Wire-Bus und speichert deren ROM-IDs
int sensorDS18B20SearchRom(void)
{
    int result = OK;

    //Suche alle Geräte auf dem Bus
    result = scanOneWireBus(foundDeviceIds, &numberOfDevices);
    if (result != OK) {
        return result;
    }

    //ROM-IDs übernehmen und auf dem Display anzeigen
    for (int i = 0; i < numberOfDevices; i++) {
        sensors[i].romID = foundDeviceIds[i];
        drawInfo("DS18B20", sensors[i].romID, i + 1);
    }

    return OK;
}

//Wandelt die Scratchpad-Daten in die Sensor-Datenstruktur um
 
static int sensorDS18B20ParseScratchpad(int sensorIndex)
{
    sensors[sensorIndex].raw.temperature =
        scratchpadData[0] | (scratchpadData[1] << 8);

    sensors[sensorIndex].raw.th        = scratchpadData[2];
    sensors[sensorIndex].raw.tl        = scratchpadData[3];
    sensors[sensorIndex].raw.config    = scratchpadData[4];
    sensors[sensorIndex].raw.reserved1 = scratchpadData[5];
    sensors[sensorIndex].raw.reserved2 = scratchpadData[6];
    sensors[sensorIndex].raw.reserved3 = scratchpadData[7];
    sensors[sensorIndex].raw.crc       = scratchpadData[8];

    return OK;
}

//Liest die Scratchpad-Daten eines einzelnen Sensors.
 
static int sensorDS18B20ReadScratchpad(int sensorIndex)
{
    uint8_t readByte = 0;
    int result = OK;

    //READ SCRATCHPAD Befehl 
    oneWireWriteByte(0xBE);

    //9 Bytes auslesen
    for (int i = 0; i < 9; i++) {
        result = oneWireReadByte(&readByte);
        if (result != OK) {
            return result;
        }
        scratchpadData[i] = readByte;
    }

    //CRC prüfen
    if (!checkCRC(sizeof(scratchpadData), scratchpadData)) {
        return ERR_BIT_FLIPPED;
    }

    //Daten übernehmen 
    return sensorDS18B20ParseScratchpad(sensorIndex);
}

/*
 * Hauptfunktion zur Temperaturmessung und Anzeige.
 * Diese Funktion wird zyklisch aus main() aufgerufen.
 */
int sensorDS18B20GetTemperature(void)
{
    int result = OK;

    //Wenn noch keine neue Messung gestartet wurde
    if (!temperatureReadDone) {

        //Alle Sensoren gleichzeitig starten 
        oneWireReset();
        oneWireWriteByte(0xCC); // SKIP ROM
        oneWireWriteByte(0x44); // CONVERT T

        //Starker Pull-Up für Messung
        sensorFullThrottle();

        //Jeden Sensor einzeln auslesen
        for (int i = 0; i < numberOfDevices; i++) {

            result = sensorSelect(sensors[i].romID);
            if (result != OK) {
                return result;
            }

            result = sensorDS18B20ReadScratchpad(i);
            if (result != OK) {
                return result;
            }
        }

        //Rohwerte in Grad Celsius umrechnen 
        result = tempCalcCelciusDS18B20(sensors, numberOfDevices);
        if (result != OK) {
            return result;
        }

        // Werte für Anzeige vorbereiten
        convertAllValuesToDisplay(sensors, numberOfDevices);

        temperatureReadDone = true;
    }

    //Temperaturen schrittweise auf dem Display anzeigen 
    if (temperatureReadDone) {
        drawTemperatureDS18B20(numberOfDevices, &temperatureReadDone);
    }

    return OK;
}
