#include "input.h"
#include "gpio.h"
#include "timer.h"
#include <stdint.h>


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

    // step == 0 bedeutet: kein gültiger Schritt erkannt
    if (step == 0) {

        // Wenn sich die Phase geändert hat, aber die Tabelle sagt "ungültig",
        // dann ist das meistens Bounce/Noise oder ein Sprung über mehrere Zustände.
        if (phase != last_phase) {

            // Wir zählen ungültige Übergänge in einem kurzen Zeitfenster,
            // damit nicht jeder einzelne Glitch direkt D21 (Fehler) auslöst.
            if (invalid_window_start == 0) {
                // Fenster starten
                invalid_window_start = now;
                invalid_cnt = 1;
            } else {
                // Wir rechnen mit Timer-Ticks statt HAL_GetTick(), damit alles im selben Zeitmaß bleibt.
                // 50ms = 50 * 1000us
                if ((now - invalid_window_start) > (uint32_t)(50 * 1000 * TICKS_PER_US)) {
                    // Fenster ist abgelaufen -> neu starten
                    invalid_window_start = now;
                    invalid_cnt = 1;
                } else {
                    // Fenster läuft noch -> hochzählen
                    if (invalid_cnt < 255) invalid_cnt++;
                }
            }

            
           
            // Dadurch ist das System "robust" und schaltet nicht bei jeder Kleinigkeit auf Fehler.
            if (invalid_cnt >= 10) {
                encoder_error = 1;
            }
        }

       
        // Auch bei ungültigem Übergang setzen wir last_phase auf den aktuellen Wert,
        // damit wir uns wieder "fangen" und nicht in einer Kette aus ungültigen Zuständen hängen bleiben.
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
