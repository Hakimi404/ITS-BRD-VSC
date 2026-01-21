#ifndef ERROR_DREHGEBER_H
#define ERROR_DREHGEBER_H

// Fehlercodes für Aufgabe 5.
// Aktuell nutzen wir hier vor allem "ungültige Übergänge" beim Drehgeber,
// z.B. durch Bounce/Noise oder wenn Zustände übersprungen werden.
#define ERROR_INVALID_TRANSITION 1

// Zentrale Fehlerbehandlung.
// Übergib einen Fehlercode (z.B. ERROR_INVALID_TRANSITION).
// Die Funktion kann z.B. D21 einschalten und warten, bis ein Reset passiert.
int error_number(int fehler);

#endif
