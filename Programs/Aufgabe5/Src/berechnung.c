#include "berechnung.h"
#include "output.h"
#include "input.h"
#include "timer.h"
#include <math.h>

void reset_system(void)
{
    encoder_phase_count = 0;
    encoder_timestamp = 0;
    encoder_error = 0;
    led_keine_aenderung();
    led_fehler_reset();
}

int getphasen(void)
{
    return (int)encoder_phase_count;
}

double get_winkel(void)
{
    return fabs((double)encoder_phase_count * SCHLITZE);
}

double get_winkelgeschw(uint32_t timestamp_ticks, double winkel, bool change)
{
    static uint32_t alt_zeit = 0;
    static double alt_winkel = 0.0;
    static double letzte_geschw = 0.0;

    // Wenn keine Änderung stattfand -> gib letzten Wert zurück
    if (!change) {
        return letzte_geschw;
    }

    // Zeitdifferenz in Sekunden
    double dt = (timestamp_ticks - alt_zeit) / (TICKS_PER_US * 1e6);

    // Division durch 0 vermeiden
    if (dt <= 0.0) {
        return letzte_geschw;
    }

    // Very small dt values create huge spikes (e.g., 1500+). Clamp for stable demo
    if (dt < 0.002) { // 2 ms
        return letzte_geschw;
    }

    double dw = fabs(winkel - alt_winkel);
    double v  = dw / dt;

    // Simple smoothing (low-pass) to avoid jitter
    v = 0.8 * letzte_geschw + 0.2 * v;

    alt_zeit = timestamp_ticks;
    alt_winkel = winkel;
    letzte_geschw = v;

    return v;
}
