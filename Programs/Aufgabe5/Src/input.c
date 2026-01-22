#include "input.h"
#include "gpio.h"
#include "timer.h"
#include <stdint.h>

#define INVALID_WINDOW_MS        50U   // Zeitfenster in ms für ungültige Übergänge
#define INVALID_THRESHOLD        10U   // ab wie vielen ungültigen Übergängen -> Fehler
#define INVALID_CNT_MAX          255U  // max Wert 


volatile uint32_t encoder_timestamp = 0;   // Zeitpunkt (Timer-Ticks) vom letzten gültigen Schritt
volatile int32_t  encoder_phase_count = 0; // Zähler für die Encoder-Schritte (+/- je nach Richtung)
volatile uint8_t  encoder_error = 0;       // Fehlerflag: 1 = zu viele ungültige Übergänge (Noise/Bounce)

// Merkt sich die letzte gelesene Phase (A/B Zustand), um Übergänge zu bewerten
static uint8_t last_phase = 0;


static uint8_t  invalid_cnt = 0;           
static uint32_t invalid_window_start = 0;  


// +1/-1 wenn ein gültiger Schritt erkannt wurde, 0 wenn kein Schritt / ungültig

static inline int8_t decode_phase(uint8_t old, uint8_t now)
{
    // Tabelle für alle 16  (4 alt-Zustände * 4 neu-Zustände)
    // 0 bedeutet: kein Schritt 
    static const int8_t table[16] = {
         0, -1,  1,  0,
         1,  0,  0, -1,
        -1,  0,  0,  1,
         0,  1, -1,  0
    };

    // (old<<2)|now baut den Index zusammen: z.B. old=2 (10b), now=1 (01b) -> 1001b = 9
    return table[(old << 2) | now];
}

// Encoder ISR logic (called from HAL_GPIO_EXTI_Callback)
// Das ist die "Herzstück"-Funktion für Aufgabe 5:
// Sie wird bei jeder Flanke (rise/fall) von S0/S1 aufgerufen (über EXTI).
void encoder_isr(void)
{
    // Wir merken uns zuerst den Zeitpunkt des Interrupts, damit später die Geschwindigkeit berechnet werden kann.
    uint32_t now = getTimeStamp();

    // Read PF0/PF1 (S0/S1) -> phase bits
    // A und B sind hier die zwei Encoder-Leitungen.
    // readGPIOPin liefert 0 oder 1.
    
    uint8_t a = (uint8_t)readGPIOPin(BUTTON_PORT, S0);
    uint8_t b = (uint8_t)readGPIOPin(BUTTON_PORT, S1);
    
    // Phase ist die 2-Bit Kombination aus A und B.
    // Beispiel: A=1, B=0 -> phase = 2 (10b)
    uint8_t phase = (uint8_t)((a << 1) | b);
    
    // Aus "letzte Phase" und "neue Phase" bestimmen wir Richtung/Schritt
    int8_t step = decode_phase(last_phase, phase);

 if (step == 0) {
    if (phase != last_phase) {

        const uint32_t window_ticks = (uint32_t)(INVALID_WINDOW_MS * 1000U * TICKS_PER_US);

        // neues Fenster starten, wenn keines aktiv oder Fenster abgelaufen
        if (invalid_window_start == 0 || (now - invalid_window_start) > window_ticks) {
            invalid_window_start = now;
            invalid_cnt = 1;
        } else {
            if (invalid_cnt < INVALID_CNT_MAX) invalid_cnt++;
        }

        if (invalid_cnt >= INVALID_THRESHOLD) {
            encoder_error = 1;
        }
    }

    // resync damit wir nicht "festhängen"
    last_phase = phase;
    return;
}

    
    // Hier sind wir bei einem gültigen Schritt:
    // step ist +1 oder -1 -> das ist gleichzeitig die Richtung.
    encoder_phase_count += step;

    // Zeitpunkt vom letzten gültigen Schritt speichern
    encoder_timestamp = now;

    // last_phase aktualisieren, sonst stimmen die nächsten Übergänge nicht
    last_phase = phase;

    // Ein gültiger Schritt bedeutet: Signal ist ok -> Filter zurücksetzen
    invalid_cnt = 0;
    invalid_window_start = 0;
}

// Checks pushbutton S7 (active low)
// S7 ist "Reset". Beim ITS Board ist der Taster meist active-low (gedrückt = 0).
bool resetpressed(void)
{
    return (readGPIOPin(BUTTON_PORT, S7) == 0);
}
