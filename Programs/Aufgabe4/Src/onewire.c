#include "onewire.h"
#include "stm32f429xx.h"
#include "timing.h"
#include "errCodes.h"
#include "global.h"


//Führt einen Reset auf dem 1-Wire-Bus aus und prüft, ob mindestens ein Sensor antwortet (Presence Detect)
int oneWireReset(void)
{
    //PD1 als Open-Drain konfigurieren und auf HIGH setzen
    GPIOD->OTYPER &= ~PD1_MASK;
    GPIOD->BSRR = PD1_MASK;

    //PD0 kurzzeitig als Push-Pull nutzen
    GPIOD->OTYPER |= PD0_MASK;

    //Reset-Puls: Leitung für mindestens 480 µs auf LOW ziehen
    GPIOD->BSRR = PD0_MASK << 16;
    wait(480);

    //Leitung wieder freigeben (HIGH durch Pull-Up)
    GPIOD->BSRR = PD0_MASK;

    //Warten auf Presence-Puls des Sensors 
    wait(70);

    //Prüfen, ob ein Sensor die Leitung auf LOW gezogen hat 
    if ((GPIOD->IDR & PD0_MASK) != DEFAULT) {
        return ERR_NO_SENSOR;
    }

    //Restliche Reset-Zeit abwarten
    wait(410);

    return OK;
}

//Schreibt ein einzelnes Bit auf den 1-Wire-Bus.Die Bit-Werte werden ausschließlich über Zeitsteuerung codiert.
int oneWireWriteBit(int bitValue)
{
    switch (bitValue)
    {
        case 0x00:
            //Bit 0: lange LOW-Zeit 
            GPIOD->BSRR = PD0_MASK << 16;
            wait(60);
            GPIOD->BSRR = PD0_MASK;
            wait(10);
            break;

        case 0x01:
            //Bit 1: kurze LOW-Zeit 
            GPIOD->BSRR = PD0_MASK << 16;
            wait(6);
            GPIOD->BSRR = PD0_MASK;
            wait(64);
            break;

        default:
            return ERR_UNKNOWN_BIT;
    }

    return OK;
}

//Liest ein einzelnes Bit vom 1-Wire-Bus.Der Sensor entscheidet durch das Timing, ob es eine 0 oder 1 ist
int oneWireReadBit(int *bitValue)
{
    //Start des Read-Zeitschlitzes
    GPIOD->BSRR = PD0_MASK << 16;
    wait(6);

    //Leitung freigeben
    GPIOD->BSRR = PD0_MASK;
    wait(9);

    //Pegel auslesen
    if ((GPIOD->IDR & PD0_MASK) != DEFAULT) {
        *bitValue = 0x01;
    } else {
        *bitValue = 0x00;
    }

    //Rest des Zeitschlitzes abwarten
    wait(55);

    return OK;
}

//Schreibt ein komplettes Byte auf den 1-Wire-Bus (LSB zuerst).
int oneWireWriteByte(uint8_t byteValue)
{
    int result = OK;

    for (int bitIndex = 0; bitIndex < 8; bitIndex++) {
        result = oneWireWriteBit((byteValue >> bitIndex) & BASE);
        if (result != OK) {
            return result;
        }
    }

    return OK;
}

//Liest ein komplettes Byte vom 1-Wire-Bus (LSB zuerst).
int oneWireReadByte(uint8_t *byteValue)
{
    int result = OK;
    int readBit = 0;

    *byteValue = DEFAULT;

    for (int bitIndex = 0; bitIndex < 8; bitIndex++) {
        result = oneWireReadBit(&readBit);
        if (result != OK) {
            return result;
        }

        *byteValue |= (readBit << bitIndex);
    }

    return OK;
}
