#ifndef INPUT_H
#define INPUT_H

#include <stdint.h>
#include <stdbool.h>

// Gemeinsamer Encoder-Zustand (shared state):
// Diese Variablen werden in der Interrupt-Service-Routine (ISR) aktualisiert
// und in der main-loop ausgelesen.
// -> volatile ist wichtig, damit der Compiler die Werte nicht cached/optimiert.
extern volatile uint32_t encoder_timestamp;   // Zeitstempel vom letzten gültigen Schritt (Timer-Ticks)
extern volatile int32_t  encoder_phase_count; // Schrittzähler: + bei CW, - bei CCW (je nach Decode)
extern volatile uint8_t  encoder_error;       // Fehlerflag: 1 bedeutet ungültige Übergänge/Noise zu oft

// Encoder ISR Logik:
// Wird aus dem EXTI Callback (Interrupt-Kette) aufgerufen.
// Hier passiert die eigentliche Auswertung der A/B-Phasen und das Hoch-/Runterzählen.
void encoder_isr(void);

// Prüft den Reset-Taster S7.
// Active-low heißt: gedrückt -> 0, nicht gedrückt -> 1
bool resetpressed(void);

#endif
