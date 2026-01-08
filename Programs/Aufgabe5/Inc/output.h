#ifndef OUTPUT_H
#define OUTPUT_H

#include "stm32f4xx_hal.h"

void led_vorwaerts(void);
void led_rueckwaerts(void);
void led_fehler(void);
void led_keine_aenderung(void);
void led_counter(int phasen);
void led_fehler_reset(void);

#endif
