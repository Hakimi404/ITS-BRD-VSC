#ifndef OUTPUT_H
#define OUTPUT_H

#include "stm32f4xx_hal.h"

// Diese Funktionen sind für die Ausgabe zuständig:
// - Richtungs-LEDs (D22/D23)
// - Fehler-LED (D21)
// - LED-Counter auf Port D (zeigt die unteren Bits vom Zähler)

void led_vorwaerts(void);        // D23 an, D22 aus (Drehrichtung vorwärts)
void led_rueckwaerts(void);      // D22 an, D23 aus (Drehrichtung rückwärts)
void led_fehler(void);           // D21 an (Fehler anzeigen)
void led_keine_aenderung(void);  // D22 und D23 aus (keine Bewegung/neutral)
void led_counter(int phasen);    // zeigt den Zählerwert (phasen) als Bitmuster auf LEDs
void led_fehler_reset(void);     // D21 aus (Fehler zurücksetzen)

#endif
