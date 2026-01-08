#include "stm32f4xx_hal.h"
#include "init.h"
#include "lcd.h"
#include "LCD_GUI.h"
#include "fontsFLASH.h"
#include "LCD_Touch.h"
#include "input.h"
#include "berechnung.h"
#include "output.h"
#include "error_drehgeber.h"
#include "timer.h"
#include <stdio.h>

#define OUTPUT_SIZE 8

int main(void) { //Steuert den Super Loop, die Zeitmessung und die Display Ausgabe
    int input = 0, output = 0, phasen = 0, reset = 0;
    double winkel = 0, geschw = 0;
    int print_idx = 0;
    uint32_t timer_ticks = 0;

    char old_winkel[OUTPUT_SIZE] = {0};
    char old_geschw[OUTPUT_SIZE] = {0};
    char buf_winkel[OUTPUT_SIZE];
    char buf_geschw[OUTPUT_SIZE];

    // Initialisierung der Hardware
    initITSboard();
    GUI_init(DEFAULT_BRIGHTNESS);
    initTimer();

    //Bildschirm vorbereiten 
    lcdGotoXY(0, 0);
    lcdSetFont(16);
    lcdPrintS("...");
    HAL_Delay(1000);

    lcdGotoXY(0, 0);
    lcdPrintS("Winkel (Grad):");

    lcdGotoXY(0, 2);
    lcdPrintS("Geschwindigkeit (Grad/s):");

    //Hauptschleife Super loop
    while (1) {
        input = input_einlesen();
        reset = resetpressed();
        output = phasen_ueberpruefung(input, reset);
        timer_ticks = getTimeStamp();

        //Reset für S7
       if (reset) {
    reset_system();  // FSM und Zähler zurücksetzen

    //Display aufräumen
    lcdGotoXY(26, 0);
    lcdPrintS("  0.0   ");
    lcdGotoXY(26, 2);
    lcdPrintS("  0.0   ");

    // LEDs zurücksetzen
    led_keine_aenderung();
    led_fehler_reset();

    //Buffer zurücksetzen für sauberen Neustart
    for (int i = 0; i < OUTPUT_SIZE; i++) {
        old_winkel[i] = ' ';
        old_geschw[i] = ' ';
    }

    continue; //Springt zum Anfang der while Schleife
}

        //Berechnungen
        phasen = getphasen();
        winkel = get_winkel();
        geschw = get_winkelgeschw(timer_ticks, winkel, output == 0);

        if (print_idx == 0) { //Die Werte nur am Anfang eines Ausgabe Zyklus erstellen
            snprintf(buf_winkel, OUTPUT_SIZE, "%7.1f", winkel);
            snprintf(buf_geschw, OUTPUT_SIZE, "%7.1f", geschw);
        }
            //Winkel: Nur schreiben, wenn sich das Zeichen geändert hat
        if (buf_winkel[print_idx] != old_winkel[print_idx]) {
            old_winkel[print_idx] = buf_winkel[print_idx];
            lcdGotoXY(26 + print_idx, 0);
            lcdPrintC(buf_winkel[print_idx]);
        }
            //Geschwindigkeit: Nur schreiben, wenn sich das Zeichen geändert hat
        if (buf_geschw[print_idx] != old_geschw[print_idx]) {
            old_geschw[print_idx] = buf_geschw[print_idx];
            lcdGotoXY(26 + print_idx, 2);
            lcdPrintC(buf_geschw[print_idx]);
        }
            // Index für nächsten Durchlauf erhöhen
        print_idx++;
        if (print_idx == OUTPUT_SIZE - 1) {
            print_idx = 0;
        }
            //Fehlerbehandlung
        if (output == PHASEUEBERSPRUNGEN)
            error_number(output);

        led_counter(phasen); ///LED Ausgabe
    }
}
