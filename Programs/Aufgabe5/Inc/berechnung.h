#ifndef BERECHNUNG_H
#define BERECHNUNG_H

#include <stdint.h>
#include <stdbool.h>

// Encoder resolution in degrees per valid quadrature step
#define SCHLITZE 0.3

void reset_system(void);
int getphasen(void);
double get_winkel(void);

double get_winkelgeschw(uint32_t timestamp_ticks, double winkel, bool change);

#endif
