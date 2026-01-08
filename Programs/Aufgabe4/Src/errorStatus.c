#include "errorStatus.h"
#include "stm32f429xx.h"
#include "timing.h"
#include "errCodes.h"

// Diese Datei ist für die Fehleranzeige zuständig. Wenn ein Fehler auftritt, wird eine LED dauerhaft blinken gelassen. Je nach Fehlercode blinkt eine andere LED.
 
// Basiswert für die LEDs (GPIOE) 
#define ERROR_LED_BASE (1)

/*
 * Zeigt einen Fehler über LEDs an.
 * Das Programm bleibt anschließend in einer Endlosschleife stehen.
 */
void indicateError(int errorCode)
{
    int ledMask = 0;

    //Je nach Fehlercode eine andere LED auswählen 
    switch (errorCode)
    {
        case ERR_UNKNOWN_BIT:
            //Fehler beim Schreiben/Lesen eines Bits
            ledMask = ERROR_LED_BASE << 1;
            break;

        case ERR_BIT_FLIPPED:
            //CRC-Fehler: Daten wurden verfälscht
            ledMask = ERROR_LED_BASE << 2;
            break;

        case ERR_NO_SENSOR:
            //Kein Sensor auf dem 1-Wire-Bus gefunden
            ledMask = ERROR_LED_BASE << 7;
            break;

        default:
            //Unbekannter Fehler
            ledMask = ERROR_LED_BASE;
            break;
    }

    //Endlosschleife: LED blinkt dauerhaft
    while (1)
    {
        //LED einschalten
        wait(1000000);
        GPIOE->BSRR = ledMask;

        //LED ausschalten
        wait(1000000);
        GPIOE->BSRR = ledMask << 16;
    }
}
