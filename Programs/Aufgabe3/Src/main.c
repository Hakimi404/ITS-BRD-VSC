#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdio.h>  
 
// Projekt-Header
#include "init.h"
#include "LCD_GUI.h"
#include "LCD_Touch.h"
#include "fontsFLASH.h"
 
#include "input.h"
#include "headers.h"
#include "bmp_reader.h"
#include "lcd_output.h"
#include "scaler.h"
#include "gpio.h"
#include "errorhandler.h"
 
//Hardware Konfiguration
#define S0_PORT GPIOF
#define S0_PIN  0
 
//Display & Speicher Konfiguration
#define LCD_WIDTH  480
#define LCD_HEIGHT 320
#define MAX_BMP_WIDTH 2400 //Maximal erlaubte BMP-Breite für unser System
#define RING_BUFFER_SIZE 6 //Anzahl der Zeilen im Ring-Puffer (Teil C)
 
//Statische Puffer (um Stack-Overflow zu vermeiden)
// Diese Variablen liegen im BSS-Segment (RAM), deswegen kein Stack-overflow möglich
static uint8_t rowBuffer[RING_BUFFER_SIZE][MAX_BMP_WIDTH]; //rowBuffer[r][x] = r-te Quellzeile, bis zu 2400 Pixel breit
static uint8_t *scalerRows[RING_BUFFER_SIZE]; //Pointer für die Zeilen, die an den Scaler übergeben werden
static uint16_t outputLine[LCD_WIDTH];  //Ausgabezeile für LCD_WriteLine()
 
//Taster-Hilfsfunktion
int button_pressed() {
    return readGPIOPin(S0_PORT, S0_PIN) == 0; //Taste S0 liegt auf Port F Pin 0
}
 
int main(void) //hauptprogramm
{
    // Hardware Initialisierung
    initITSboard();
    GUI_init(DEFAULT_BRIGHTNESS);
    TP_Init(false);
 
    if (!checkVersionFlashFonts()) {
        LOOP_ON_ERR(true, "Font Mismatch");
    }
 
    initInput();
    GUI_clear(BLACK);
 
    while (1)
    {
        //Warten auf Nutzer, Button muss gedrückt und wieder losgelassen werden
        while (!button_pressed()); 
        while (button_pressed());  
       
        openNextFile(); //Neues File im Python-Programm anfordern
        GUI_clear(BLACK);
 
        //Variablen für Header anlegen
        BITMAPFILEHEADER fh;
        BITMAPINFOHEADER ih;
       
        // ÄNDERUNG: Statisches Array statt malloc.
        // Belegt festen Speicher (1KB) und verhindert Fragmentierung/Lecks.
        static RGBQUAD pal[256];
 
        // 3. Header einlesen
        // bmp_start füllt nun das statische Array 'pal'
        if (bmp_start(&fh, &ih, pal) != EOK) {
            // Kein free(pal) mehr nötig!
           
            //Warten, damit Fehlermeldung gelesen werden kann
            while (!button_pressed());
            while (button_pressed());
            continue;
        }
 
        int srcW = ih.biWidth;
        int srcH = ih.biHeight;
 
        //Sicherheitsprüfung: Bild darf nicht breiter sein, als das RAM erlaubt
        if (srcW > MAX_BMP_WIDTH) {
            lcdErrorMsg("Bild zu breit!");
            
            while (!button_pressed());
            while (button_pressed());
            continue;
        }
 
        
        //SKALIERUNGS-BERECHNUNG (Fit-to-Screen)
        float scale_x = (float)LCD_WIDTH / (float)srcW;
        float scale_y = (float)LCD_HEIGHT / (float)srcH;
 
        //Kleineren Faktor wählen → Bild vollständig sichtbar
        float scale = (scale_x < scale_y) ? scale_x : scale_y;
 
        // Kein Upscaling (das ist hier optional)
        if (scale > 1.0f) scale = 1.0f;
 
        //Größe des angezeigten Bildes
        int displayImageWidth  = (int)(srcW * scale);
        int displayImageHeight = (int)(srcH * scale);
 
        //Zentrierung durch Letterboxing
        int offsetX = (LCD_WIDTH - displayImageWidth) / 2;
        int offsetY = (LCD_HEIGHT - displayImageHeight) / 2;
 
        //Box-Größe (Wie viele Quellzeilen pro Zielzeile?)
        int boxSize = (int)ceilf(1.0f / scale);
        if (boxSize > RING_BUFFER_SIZE) boxSize = RING_BUFFER_SIZE;
        if (boxSize < 1) boxSize = 1;
 

        //STREAMING LOOP (Teilaufgabe C) / Zeilenweise lesen → skalieren → anzeigen
        int rowsReadTotal = 0;
        int bmpStatus = 0; // 0 = OK, -1 = Fehler/EOF
 
        // Iteration über die HÖHE des Zielbildes
        for (int i = 0; i < displayImageHeight; i++)
        {
            //Welche Zeilen brauchen wir aus dem Quellbild?
            float srcY_start_f = (float)i / scale;
           
            int startSrcRow = (int)floorf(srcY_start_f);
            int endSrcRow   = startSrcRow + boxSize;
 
            if (endSrcRow > srcH) endSrcRow = srcH;
 
            //B. Ringpuffer nachladen
            //Solange lesen, bis wir genug Zeilen für die Box-Berechnung haben
            while (rowsReadTotal < endSrcRow && bmpStatus == 0)
            {
                uint8_t *bufPtr = rowBuffer[rowsReadTotal % RING_BUFFER_SIZE];
               
                //Zeile einlesen (ruft RLE oder RAW Logik auf)
                int res = bmp_read_row(bufPtr, srcW);
                if (res != 0) {
                    bmpStatus = -1; // Abbruch markieren
                }
                rowsReadTotal++;
            }
 
            //Zeiger für den Scaler vorbereiten
            int validRows = 0;
            for (int r = startSrcRow; r < endSrcRow; r++)
            {
                // Nur Zeilen nutzen, die wir erfolgreich gelesen haben
                if (r < rowsReadTotal) {
                    scalerRows[validRows] = rowBuffer[r % RING_BUFFER_SIZE];
                    validRows++;
                }
            }
 
            //Skalierung + Ausgabe der fertigen LCD-Zeile
            if (validRows > 0)
            {
                // Aufruf der Skalierungsfunktion
                scale_line_box_fit(outputLine, scalerRows, validRows,
                                   srcW, scale, offsetX, displayImageWidth, pal);
               
                // Y-Position berechnen (BMP ist Bottom-Up!)
                // Wir zeichnen von unten nach oben auf das Display
                int lcdY = (offsetY + displayImageHeight - 1) - i;
               
                // Zeichnen (Ganze Zeile wird geschrieben)
                lcd_draw_row(0, lcdY, outputLine, LCD_WIDTH);
            }
        }
 
        //Restliche Zeilen auslesen (damit UART Puffer leer ist für nächstes Bild)
        while (rowsReadTotal < srcH && bmpStatus == 0) {
            bmp_read_row(rowBuffer[0], srcW);
            rowsReadTotal++;
        }
 
        // Warten auf User Eingabe für das nächste Bild
        while (!button_pressed());
        while (button_pressed());
    }
}