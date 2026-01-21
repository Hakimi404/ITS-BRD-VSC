#ifndef BERECHNUNG_H
#define BERECHNUNG_H

#include <stdint.h>
#include <stdbool.h>

// Das ist die Auflösung vom Drehgeber in Grad pro gültigem Quadratur-Schritt.
// Bedeutet: bei jedem erkannten Schritt (step = +1/-1) ändert sich der Winkel um 0.3°.
// (Je nach Encoder/Signalquelle kann dieser Wert auch anders sein.)
#define SCHLITZE 0.3

// Setzt das komplette System zurück:
// - Zähler wieder auf 0
// - Fehler/LEDs zurücksetzen
// - interner Zustand wieder "Start"
void reset_system(void);

// Gibt den aktuellen Schritt-/Phasen-Zähler zurück (wie viele Schritte seit Start/Reset).
// Kann positiv oder negativ sein (je nach Drehrichtung).
int getphasen(void);

// Rechnet aus dem Schrittzähler den Winkel in Grad aus.
// Typisch: |phasen| * SCHLITZE
double get_winkel(void);

// Rechnet die Winkelgeschwindigkeit in Grad/s aus.
// timestamp_ticks: Zeitstempel vom letzten gültigen Schritt (Timer-Ticks)
// winkel: aktueller Winkel
// change: true wenn sich etwas bewegt hat (sonst würde der alte Wert "stehen bleiben")
double get_winkelgeschw(uint32_t timestamp_ticks, double winkel, bool change);

#endif
