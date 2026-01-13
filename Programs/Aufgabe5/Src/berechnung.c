#include "berechnung.h"
#include "output.h"
#include "error_drehgeber.h"
#include "timer.h"
#include "input.h"
#include <math.h>

const StateType delta[14][4] = {
    [Start]     = { ANoRot, DNoRot, BNoRot, CNoRot },
    [Err]       = { Err, Err, Err, Err },
    [AForRot]   = { AForRot, DBackRot, BForRot, Err },
    [BForRot]   = { ABackRot, Err, BForRot, CForRot },
    [CForRot]   = { Err, DForRot, BBackRot, CForRot },
    [DForRot]   = { AForRot, DForRot, Err, CBackRot },
    [ABackRot]  = { ABackRot, DBackRot, BForRot, Err },
    [BBackRot]  = { ABackRot, Err, BBackRot, CForRot },
    [CBackRot]  = { Err, DForRot, BBackRot, CBackRot },
    [DBackRot]  = { AForRot, DBackRot, Err, CBackRot },
    [ANoRot]    = { ANoRot, DBackRot, BForRot, Err },
    [BNoRot]    = { ABackRot, Err, BNoRot, CForRot },
    [CNoRot]    = { Err, DForRot, BBackRot, CNoRot },
    [DNoRot]    = { AForRot, DNoRot, Err, CBackRot }
};

int state = Start;
int prev = 0;

void reset_system(void)
{
    state = Start;
    prev = 0;
    encoder_phase_count = 0;
    led_keine_aenderung();
    led_fehler_reset();
}

int getphasen(void)
{
    return encoder_phase_count;
}

double get_winkel(void)
{
    return fabs(encoder_phase_count * SCHLITZE);
}

double get_winkelgeschw(uint32_t timestamp, double winkel, bool change)
{
    static uint32_t alt_zeit = 0;
    static double alt_winkel = 0.0;
    static double letzte_geschw = 0.0;

    if (!change) return letzte_geschw;

    double dt = (timestamp - alt_zeit) / (TICKS_PER_US * 1e6);
    if (dt <= 0) return letzte_geschw;

    double dw = fabs(winkel - alt_winkel);
    double v = dw / dt;

    alt_zeit = timestamp;
    alt_winkel = winkel;
    letzte_geschw = v;

    return v;
}
