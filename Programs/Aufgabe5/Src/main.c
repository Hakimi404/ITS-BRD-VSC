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
#include "gpio.h"
#include <stdio.h>

#define OUTPUT_SIZE 8

int main(void)
{
    /* application values */
    int phasen = 0;
    double winkel = 0.0;
    double geschw = 0.0;
    int reset = 0;
    int print_idx = 0;

    /* double-read variables (ISR safety) */
    uint32_t ts1, ts2;
    int32_t  c1, c2;
    uint32_t timestamp = 0;
    int tries = 0;

    /* display buffers */
    char old_winkel[OUTPUT_SIZE] = {0};
    char old_geschw[OUTPUT_SIZE] = {0};
    char buf_winkel[OUTPUT_SIZE];
    char buf_geschw[OUTPUT_SIZE];

    //Hardware init 
    initITSboard();
    initEncoderInterrupts();      
    GUI_init(DEFAULT_BRIGHTNESS);
    initTimer();

    //Display init
    lcdGotoXY(0, 0);
    lcdSetFont(16);
    lcdPrintS("...");
    HAL_Delay(1000);

    lcdGotoXY(0, 0);
    lcdPrintS("Winkel (Grad):");

    lcdGotoXY(0, 2);
    lcdPrintS("Geschwindigkeit (Grad/s):");

    //MAIN LOOP 
    while (1)
    {
        // Reset button S7 (polling allowed)
        reset = resetpressed();
        if (reset)
        {
            reset_system();

            lcdGotoXY(26, 0);
            lcdPrintS("  0.0   ");
            lcdGotoXY(26, 2);
            lcdPrintS("  0.0   ");

            led_keine_aenderung();
            led_fehler_reset();

            for (int i = 0; i < OUTPUT_SIZE; i++) {
                old_winkel[i] = ' ';
                old_geschw[i] = ' ';
            }
            continue;
        }

        // Double-read protection (MANDATORY)
        tries = 0;
        do {
            ts1 = encoder_timestamp;
            c1  = encoder_phase_count;
            ts2 = encoder_timestamp;
            c2  = encoder_phase_count;
            tries++;
        } while ((ts1 != ts2 || c1 != c2) && tries < 10);

        if (tries >= 10) {
            error_number(PHASEUEBERSPRUNGEN);
            continue;
        }

        timestamp = ts1;
        phasen    = c1;

        // Calculations
        winkel = get_winkel();
        geschw = get_winkelgeschw(timestamp, winkel, 1);

        // Display update (unchanged logic)
        if (print_idx == 0) {
            snprintf(buf_winkel, OUTPUT_SIZE, "%7.1f", winkel);
            snprintf(buf_geschw, OUTPUT_SIZE, "%7.1f", geschw);
        }

        if (buf_winkel[print_idx] != old_winkel[print_idx]) {
            old_winkel[print_idx] = buf_winkel[print_idx];
            lcdGotoXY(26 + print_idx, 0);
            lcdPrintC(buf_winkel[print_idx]);
        }

        if (buf_geschw[print_idx] != old_geschw[print_idx]) {
            old_geschw[print_idx] = buf_geschw[print_idx];
            lcdGotoXY(26 + print_idx, 2);
            lcdPrintC(buf_geschw[print_idx]);
        }

        print_idx++;
        if (print_idx == OUTPUT_SIZE - 1)
            print_idx = 0;

        //LED output
        led_counter(phasen);
    }
}
