#include "error_drehgeber.h"
#include "gpio.h"
#include "output.h"
#include "berechnung.h"

//Behandelt Fehler, blockiert bei Phasenfehler das System bis S6 gedrückt wird
int error_number(int fehler) {  
    led_fehler(); // D21 EIN
    if (fehler == PHASEUEBERSPRUNGEN) {
        while (1) { //Endlosschleife bis zum manuellen Reset
            int s6 = readGPIOPin(BUTTON_PORT, S6);
            if (s6 == 0) {  // System zurücksetzen, wenn S6 gedrückt
                led_fehler_reset();   // D21 AUS
                reset_system();       // FSM & Zähler zurücksetzen
                return 0;
            }
        }
    }
    return 0;
}
