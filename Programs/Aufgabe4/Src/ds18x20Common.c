#include "ds18x20Common.h"
#include "crc.h"
#include "display.h"
#include "global.h"

#include <stdbool.h>
#include <stdint.h>



//Dauer für den starken Pull-Up (wichtig für Temperaturmessung)
#define STRONG_PULLUP_TIME 750000

//Variablen für den Search-ROM-Algorithmus
static uint64_t previousRomId = 0;
static uint64_t currentRomId  = 0;

static int lastConflictBit    = -1;
static int newConflictBit     = -1;

static bool lastDeviceFound   = false;

//Puffer für CRC-Berechnung
static unsigned char crcBuffer[8];

// Aktiviert einen starken Pull-Up auf dem 1-Wire-Bus Der DS18B20 benötigt während der Temperaturmessung mehr Strom, als der normale Pull-Up liefern kann
 
void sensorFullThrottle(void)
{
    //PD0 kurzzeitig als Push-Pull schalten 
    GPIOD->OTYPER &= ~PD0_MASK;

    //Leitung aktiv auf HIGH ziehen 
    GPIOD->BSRR = PD0_MASK;

    //Starken Pull-Up für eine bestimmte Zeit halten
    wait(STRONG_PULLUP_TIME);

    //PD0 wieder auf Open-Drain zurückschalten
    GPIOD->OTYPER |= PD0_MASK;
}

/*
 * Wählt genau einen Sensor anhand seiner 64-Bit-ROM-ID aus
 * Nur dieser Sensor reagiert danach auf weitere Befehle
 */
int sensorSelect(uint64_t romId)
{
    int result = OK;

    // 1-Wire-Bus zurücksetzen 
    result = oneWireReset();
    if (result != OK) {
        return result;
    }

    //MATCH ROM Befehl senden
    oneWireWriteByte(0x55);

    //ROM-ID Byte für Byte (LSB zuerst) senden
    for (int byteIndex = 0; byteIndex < 8; byteIndex++) {
        result = oneWireWriteByte((uint8_t)(romId >> (8 * byteIndex)));
        if (result != OK) {
            return result;
        }
    }

    return OK;
}

//Durchsucht den 1-Wire-Bus nach allen angeschlossenen Sensoren. Die gefundenen ROM-IDs werden im Array deviceIDs gespeichert
 int scanOneWireBus(uint64_t *deviceIDs, int *deviceCount)
{
    int result = OK;

    //Suchzustand zurücksetzen
    lastDeviceFound = false;
    previousRomId   = 0;
    currentRomId    = 0;
    lastConflictBit = -1;

    do {
        int bitZero = 0;
        int bitOne  = 0;
        uint64_t selectedBit = 0;

        //Bus vor jedem Suchdurchlauf zurücksetzen
        result = oneWireReset();
        if (result != OK) {
            return result;
        }

        //SEARCH ROM Befehl senden
        result = oneWireWriteByte(0xF0);
        if (result != OK) {
            return result;
        }

        //Vorbereitung für neue ROM-ID
        previousRomId = currentRomId;
        currentRomId  = 0;
        newConflictBit = -1;

        //Alle 64 Bits der ROM-ID abarbeiten
        for (int bitIndex = 0; bitIndex < 64; bitIndex++) {

            // Beide möglichen Bitwerte lesen
            oneWireReadBit(&bitZero);
            oneWireReadBit(&bitOne);

            //Kein Sensor vorhanden
            if (bitZero == 1 && bitOne == 1) {
                return ERR_NO_SENSOR;
            }

            //Kein Konflikt → eindeutige Richtung
            if (bitZero != bitOne) {
                selectedBit = bitZero;
            }
            else {
                //Konflikt: mindestens zwei Sensoren
                if (bitIndex < lastConflictBit) {
                    selectedBit = (previousRomId >> bitIndex) & 0x01U;
                }
                else if (bitIndex == lastConflictBit) {
                    selectedBit = 1;
                }
                else {
                    selectedBit = 0;
                }

                //Neue Konfliktstelle merken
                if (selectedBit == 0) {
                    newConflictBit = bitIndex;
                }
            }

            //Gewähltes Bit auf den Bus schreiben
            oneWireWriteBit(selectedBit);

            // Bit in aktueller ROM-ID speichern
            currentRomId |= (selectedBit << bitIndex);
        }

        //Konfliktposition aktualisieren 
        lastConflictBit = newConflictBit;

        //Gefundene ROM-ID speichern
        if (*deviceCount < MAX_SUPPORTET_DEVICES) {
            deviceIDs[*deviceCount] = currentRomId;
            (*deviceCount)++;
        }

        //Keine weiteren Konflikte --> letztes Gerät gefunden
        if (lastConflictBit == -1) {
            lastDeviceFound = true;
        }

    } while (!lastDeviceFound);

    return OK;
}
