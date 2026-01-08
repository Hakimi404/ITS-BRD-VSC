#ifndef BERECHNUNG_H
#define BERECHNUNG_H

#include <stdint.h>
#include <stdbool.h>

#define PHASE_A 0
#define PHASE_B 1
#define PHASE_C 2
#define PHASE_D 3

#define SCHLITZE 0.3

typedef enum {
    Start = 0, Err = 1,
    AForRot = 2, ABackRot = 3, ANoRot = 4,
    BForRot = 5, BBackRot = 6, BNoRot = 7,
    CForRot = 8, CBackRot = 9, CNoRot = 10,
    DForRot = 11, DBackRot = 12, DNoRot = 13
} StateType;

extern const StateType delta[14][4];

void reset_system(void);
int phasen_ueberpruefung(int input, int resetpressed);
int getphasen(void);
double get_winkel(void);
double get_winkelgeschw(uint32_t timer_ticks, double winkel, bool change);

#endif

