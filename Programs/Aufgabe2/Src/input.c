#include "input.h"
#include "gpio.h"
#include "berechnung.h"
#include "timer.h"
#include <stdbool.h>

#define MAX 90000000
unsigned int aft = 0, bef = 0, time = 0;

int input_einlesen() {
    aft = TIM2->CNT; // Den aktuellen Timer Wert holen
    time += aft - bef; // Die verstrichene Zeit aufaddieren
    bef = aft; // Der Referenzwert für den nächsten Aufruf setzen

    /*int s0 = readGPIOPin(BUTTON_PORT, S0);
    int s1 = readGPIOPin(BUTTON_PORT, S1);

    if (!s0 && !s1) return PHASE_A;
    if (s0 && !s1)  return PHASE_B;
    if (s0 && s1)   return PHASE_C;
    if (!s0 && s1)  return PHASE_D;*/
    return GPIOF->IDR & 0x03; //Maskiert die unteren 2 Bits (IN0, IN1) von Port F
}
//Prüft ob das Zeitlimit überschritten wurde
int inputzeit() {
    if (time >= MAX) { time = 0; return 1; }
    return 0;
}
//Prüft Taster S7
bool resetpressed() {
    int s7 = readGPIOPin(BUTTON_PORT, S7);
    return (s7 == 0);
}
