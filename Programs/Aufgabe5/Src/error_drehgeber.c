#include "error_drehgeber.h"
#include "gpio.h"
#include "output.h"
#include "berechnung.h"

//Behandelt Fehler, blockiert bei Encoder-Fehler das System bis S6 gedrückt wird
int error_number(int fehler)
{
    (void)fehler;

    led_fehler(); // D21 EIN

    while (1) {
        int s6 = readGPIOPin(BUTTON_PORT, S6);
        if (s6 == 0) {
            led_fehler_reset();
            reset_system();
            return 0;
        }
    }
}
