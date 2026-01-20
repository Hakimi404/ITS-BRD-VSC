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

// Direction LED hold (prevents visible blinking when encoder is moved slowly)
uint32_t dir_hold_until = 0;
int last_dir = 0; // +1 forward, -1 backward, 0 none

int main(void)
{
    //application values
    int phasen = 0;
    double winkel = 0.0;
    double geschw = 0.0;
    int reset = 0;
    int print_idx = 0;

    //step-based tracking (A5)
    int32_t last_phase_count = 0;
    int step = 0;

    //Speed zero timeout (if no movement for some time -> speed = 0)
    uint32_t last_move_ms = HAL_GetTick();

    //double-read variables (ISR sicherheit)
    uint32_t ts1, ts2;
    int32_t  c1, c2;
    uint8_t  e1, e2;
    uint32_t timestamp = 0;
    int tries = 0;

    //display buffers
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

            last_phase_count = 0;
            last_move_ms = HAL_GetTick();
            continue;
        }

        // Double-read protection (ISR sicherheit)
        tries = 0;
        do {
            ts1 = encoder_timestamp;
            c1  = encoder_phase_count;
            e1  = encoder_error;

            ts2 = encoder_timestamp;
            c2  = encoder_phase_count;
            e2  = encoder_error;
            tries++;
        } while ((ts1 != ts2 || c1 != c2 || e1 != e2) && tries < 10);

        if (tries >= 10) {
            error_number(ERROR_INVALID_TRANSITION);
            continue;
        }

        // If ISR detected invalid transition/bounce -> go error
        if (e1 != 0) {
            error_number(ERROR_INVALID_TRANSITION);
            continue;
        }

        timestamp = ts1;
        phasen    = (int)c1;

        //STEP calculation (A5 core)
        step = (int)(c1 - last_phase_count);
        if (step > 0)
            step = 1;
        else if (step < 0)
            step = -1;
        else
            step = 0;

        last_phase_count = c1;

        //Remember last movement time (for speed -> 0 behavior)
        if (step != 0) {
            last_move_ms = HAL_GetTick();
        }

        //Direction LEDs (with hold time)
        if (step > 0) {
            last_dir = 1;
            dir_hold_until = HAL_GetTick() + 150;
        } else if (step < 0) {
            last_dir = -1;
            dir_hold_until = HAL_GetTick() + 150;
        }

        if ((int32_t)(HAL_GetTick() - dir_hold_until) <= 0) {
            if (last_dir > 0)
                led_vorwaerts();
            else if (last_dir < 0)
                led_rueckwaerts();
            else
                led_keine_aenderung();
        } else {
            led_keine_aenderung();
            last_dir = 0;
        }

        //Calculations
        winkel = get_winkel();
        geschw = get_winkelgeschw(timestamp, winkel, (step != 0));

        //If no movement for 200ms -> force speed to 0.0
        if ((HAL_GetTick() - last_move_ms) > 200) {
            geschw = 0.0;
        }

        //Display update
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

        //LED counter output
        led_counter(phasen);
    }
}
