#include "errorStatus.h"
#include "timing.h"
#include "errCodes.h"
#include "display.h"
#include <stdio.h>

// Diese Datei ist für die Fehleranzeige zuständig. Wenn ein Fehler auftritt, wird eine LED dauerhaft blinken gelassen. Je nach Fehlercode blinkt eine andere LED.
 
// Basiswert für die LEDs (GPIOE) 
#define ERROR_LED_BASE (1)
#define ERROR_DISPLAYTIME (5 * US_TO_S)
#define US_TO_S 1000000
#define ERRCODE_STR_MAXLEN 5

/*
 * Zeigt einen Fehler über LEDs an.
 * Das Programm bleibt anschließend in einer Endlosschleife stehen.
 */
void indicateError(int errorCode)
{
    char errCodeS[ERRCODE_STR_MAXLEN];
    int ledMask = 0;

    //Je nach Fehlercode eine andere LED auswählen 
    switch (errorCode)
    {
        case ERR_UNKNOWN_BIT:
            writeToError("UNKNOWN_BIT", "Fehler beim Lesen/Schreiben eines Bits");
            wait(ERROR_DISPLAYTIME);
            clearError();
            break;
        case ERR_BIT_FLIPPED:
            writeToError("ERR_BIT_FLIPPED", "Daten wurden Verfaelscht");
            wait(ERROR_DISPLAYTIME);
            clearError();
            break;
        case ERR_NO_SENSOR:
            writeToError("NO_SENSOR", "Kein Sensor auf dem 1-Wire-Bus gefunden");
            wait(ERROR_DISPLAYTIME);
            clearError();
            ledMask = ERROR_LED_BASE << 7;
            break;

        default:
            sprintf(errCodeS, "%d", errorCode);
            writeToError(errCodeS, "Fehlercode stimmt mit keinem Bekannten Fehlercode ueberein");
            wait(ERROR_DISPLAYTIME);
            clearError();
            ledMask = ERROR_LED_BASE;
            break;
    }
    /*
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
    */
}
