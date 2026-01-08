#include "fehler.h"
#include "display.h"


void errorHandler(int err) {  // Fehler-Modus (rote Farbe)
    setErrMode();

    switch (err) {
        case ERR_STACK_EMPTY:
            printStdout("Fehler: Stack ist leer.\n");
            break;
        case ERR_STACK_FULL:
            printStdout("Fehler: Stack ist voll.\n");
            break;
        case ERR_DIV_ZERO:
            printStdout("Fehler: Division durch 0!\n");
            break;
        case ERR_OVERFLOW:
            printStdout("Fehler: Bereichsueberschreitung!\n");
            break;
        case ERR_UNKNOWN:
            printStdout("Fehler: Ungueltige Eingabe!\n");
            break;
        default:
            setNormalMode();
            break;
    }
}