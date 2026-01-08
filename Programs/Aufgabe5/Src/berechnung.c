#include "berechnung.h"
#include "output.h"
#include "error_drehgeber.h"
#include "timer.h"
#include <math.h>

//Zustandsübergangstabelle
//Vereinfachte Zustandsmatrix für 14 Zustände und 4 Eingaben
const StateType delta[14][4] = {
    // Start und Fehler
    [Start]     = { ANoRot, DNoRot, BNoRot, CNoRot },
    [Err]       = { Err, Err, Err, Err },
        // Vorwärtsrotation
    [AForRot]   = { AForRot, DBackRot, BForRot, Err },
    [BForRot]   = { ABackRot, Err, BForRot, CForRot },
    [CForRot]   = { Err, DForRot, BBackRot, CForRot },
    [DForRot]   = { AForRot, DForRot, Err, CBackRot },
        //Rückwärtsrotation
    [ABackRot]  = { ABackRot, DBackRot, BForRot, Err },
    [BBackRot]  = { ABackRot, Err, BBackRot, CForRot },
    [CBackRot]  = { Err, DForRot, BBackRot, CBackRot },
    [DBackRot]  = { AForRot, DBackRot, Err, CBackRot },
        //Keine Bewegung
    [ANoRot]    = { ANoRot, DBackRot, BForRot, Err },
    [BNoRot]    = { ABackRot, Err, BNoRot, CForRot },
    [CNoRot]    = { Err, DForRot, BBackRot, CNoRot },
    [DNoRot]    = { AForRot, DNoRot, Err, CBackRot }
};

int state = Start;
int prev = 0;
int phasen = 0;
double winkel = 0;
double geschw = 0;

// interne Zeit- und Winkelvariablen für Geschwindigkeitsberechnung
static uint32_t alt_zeit = 0;
static double alt_winkel = 0.0;
static double geschw_filter = 0.0;

//Setzt das System in den Ausgangszustand zurück
void reset_system(void) {
    prev = 0;
    phasen = 0;
    state = Start;
    winkel = 0;
    geschw = 0;
    geschw_filter = 0.0;
    alt_zeit = 0;
    alt_winkel = 0.0;
    led_keine_aenderung();
    led_fehler_reset();
}
        /*Führt den Zustandswechsel der FSM durch und steuert die LEDs
        * Rückgabe: 0 bei Änderung, 1 bei Stillstand, PHASEUEBERSPRUNGEN bei Fehler 
        */
int phasen_ueberpruefung(int input, int resetpressed) {
    state = delta[state][input];
    if (state == prev) return 1;
    prev = state;
    if (resetpressed) reset_system();

        //Aktionen basierend auf dem neuen Zustand
    switch (state) {
        case Err:
            led_fehler();   //D21 EIN
            led_keine_aenderung(); //D22 + D23 AUS bei Fehler
            return PHASEUEBERSPRUNGEN;

        case AForRot: case BForRot: case CForRot: case DForRot:
            led_vorwaerts(); // D23 AN, D22 AUS
            phasen++;
            break;

        case ABackRot: case BBackRot: case CBackRot: case DBackRot:
            led_rueckwaerts(); // D22 AN, D23 AUS
            phasen--;
            break;

        default:
            led_keine_aenderung();
            break;
    }
    return 0;
}
int getphasen(void) {
return phasen;
}
 
// Berechnet den aktuellen Winkel basierend auf den Phasen
double get_winkel(void) {
return fabs(phasen * SCHLITZE);
}
 
// Berechnet die Winkelgeschwindigkeit nur bei echter Änderung
double get_winkelgeschw(uint32_t timer_ticks, double winkel, bool change)
{
    static uint32_t alt_zeit = 0;
    static double alt_winkel = 0.0;
    static double letzte_geschw = 0.0;
    
    // Wenn keine Änderung stattfand → gib einfach den letzten Wert zurück
    if (!change) {
    return letzte_geschw;
    }
    
    //Zeitdifferenz berechnen
    double zeit_diff = (timer_ticks - alt_zeit) / (TICKS_PER_US * 1e6);
    
    //Division durch 0 vermeiden
    if (zeit_diff <= 0) {
        return letzte_geschw;
    }
    
    if (zeit_diff < 0.25) {
            return letzte_geschw;
    }

    double winkel_diff = fabs(winkel - alt_winkel);
    
    //Neue Geschwindigkeit berechnen
    double geschw_neu = winkel_diff / zeit_diff;
    
    // Wenn das System stillsteht, Geschwindigkeit = 0
    if (winkel_diff < 0.001) {
        geschw_neu = 0.0;
    }


    // die Werte speichern für nächsten Aufruf
    alt_winkel = winkel;
    alt_zeit = timer_ticks;
    letzte_geschw = geschw_neu;
    
    return geschw_neu;
}