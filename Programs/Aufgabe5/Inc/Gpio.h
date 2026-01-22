#ifndef GPIO_H
#define GPIO_H

#include "stm32f4xx_hal.h"

// ===== Port/Pin Definitionen für das ITS-Board =====

// Auf dem ITS-Board liegen die Taster/Encoder-Signale (S0, S1, S6, S7) auf Port F
#define BUTTON_PORT GPIOF

// S0/S1 sind die zwei Encoder-Leitungen (Quadratur A/B)
// Hier sind es PF0 und PF1
#define S0 0
#define S1 1

// S6/S7 sind Taster (meist active-low)
// S6 wird oft als "Fehler quittieren" genutzt, S7 als Reset
#define S6 6
#define S7 7

// D21/D22/D23 sind LEDs auf Port E (in deinem Code wird GPIOE genutzt)
// Hier sind die Bitmasken für die jeweiligen LED-Pins
#define D21 (1 << 5)
#define D22 (1 << 6)
#define D23 (1 << 7)

// ===== Register-/Bitmasken (Low-Level) =====
// Einige Makros wirken wie aus Aufgabe 2/Low-Level-Konfiguration.
// Du nutzt aktuell HAL für EXTI, aber die Defines kannst du drin lassen,
// falls ihr im Protokoll oder in anderen Teilen direkt Register anfasst.

#define PORTF_EXTICRMASK 0x05U      // Maskierung/Port-Index (EXTI Config)
#define EXTIPORT_SIZE 0x4U          // Schrittweite pro EXTI Config Feld
#define PIN_0 0x00U                 // Pin-Index 0
#define PIN_1 0x01U                 // Pin-Index 1

#define MODER_INPUT 0x00U           // MODER Wert für Input
#define MODER_BITPERPIN 0x2U        // MODER hat 2 Bits pro Pin

#define PUPDR_PULLUP 0x1U           // PUPDR Pull-Up Wert
#define PUPDR_BITPERPIN 0x2U        // PUPDR hat 2 Bits pro Pin

#define TWOBITCLEAR_MASK 0x3U       // Maske um 2 Bits sauber zu löschen

// ===== Funktionen =====

// Liest einen Pinzustand direkt aus dem GPIO-Register:
// Rückgabe: 1 = HIGH, 0 = LOW
int readGPIOPin(GPIO_TypeDef *GPIOx, int pin);

// Konfiguriert die Encoder-Eingänge (S0/S1 = PF0/PF1) als EXTI Interrupts.
// Dadurch wird bei jeder Flanke (steigend/fallend) ein Interrupt ausgelöst,
// und unsere encoder_isr() kann Schritte zählen, ohne Polling.
void initEncoderInterrupts(void);

#endif
