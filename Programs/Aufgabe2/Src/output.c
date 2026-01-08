#include "output.h"
#include "lcd.h"
#include "berechnung.h"
#include "gpio.h"
#include <stdio.h>

void led_vorwaerts() { //Signalisiert Vorwärtslauf (D23 an, D22 aus)
  HAL_GPIO_WritePin(GPIOE, D22, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOE, D23, GPIO_PIN_SET);
}

void led_rueckwaerts() { //Signalisiert Rückwärtslauf (D22 an, D23 aus)
  HAL_GPIO_WritePin(GPIOE, D23, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOE, D22, GPIO_PIN_SET);
}

void led_fehler() { //Aktiviert die Fehler LED D21
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5, GPIO_PIN_SET);  // D21 AN
}

void led_fehler_reset() { //Schaltet die Fehler LED D21 aus
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5, GPIO_PIN_RESET); // D21 AUS
}


void led_keine_aenderung() { //Schaltet die Richtungs LEDs (D22, D23) aus
  HAL_GPIO_WritePin(GPIOE, D23, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOE, D22, GPIO_PIN_RESET);
}

void led_counter(int phasen) { //Gibt die unteren 8 Bits von 'phasen' auf Port D aus
  GPIOD->BSRR = (phasen & 0xFF) | ((~phasen) << 16);
}
