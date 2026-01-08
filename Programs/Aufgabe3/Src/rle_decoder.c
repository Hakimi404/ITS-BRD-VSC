#include "rle_decoder.h"
#include "input.h"
#include "LCD_GUI.h"
#include "fontsFLASH.h"
#include <stdio.h>

static int curX = 0;
static int curY = 0;
static int height = 0;
static int ended = 0;

//rleError()   Kleine Hilfsfunktion zum Anzeigen von Fehlermeldungen am LCD
static void rleError(const char *msg)
{
    GUI_clear(WHITE);
    Coordinate tl = { 10, 10 };
    GUI_disStr(tl, msg, &Font16, WHITE, RED);
}

/*rle_reset_with_height()
 *   Wird bei jedem neuen BMP-Bild vom Reader aufgerufen.
 *   Setzt Decoder-Zustände korrekt zurück.
 */
void rle_reset_with_height(int h)
{
    height = h;
    curX = 0;
    curY = 0;
    ended = 0;
}

//rle_reset() – Reset ohne Höhe
void rle_reset(void)
{
    curX = 0;
    curY = 0;
    ended = 0;
}

//rle_decode_row()  Dekodiert EINE Zeile aus einem RLE8-komprimierten BMP.
int rle_decode_row(uint8_t *row, int width)
{
    if (ended)                  // = 0   → Zeile erfolgreich dekodiert / = 1   → End-of-Bitmap wurde erreicht (weitere Zeilen = leer) / = -1  → Fehler (EOF oder ungültiger Stream)
    {
        for (int i = 0; i < width; i++)
            row[i] = 0;
        return 1;
    }

    /* Zeile löschen */
    for (int i = 0; i < width; i++)
        row[i] = 0;

    curX = 0;

    //Hauptschleife: Wir dekodieren so lange, bis ein "End-of-Line" (00 00) kommt.
    while (curY < height)                   //c1:Anzahl Pixel  / c2:Farbindex
    {
        int c1 = nextChar();       
        int c2 = nextChar();

        if (c1 == EOF || c2 == EOF)
        {
            rleError("Fehler: RLE EOF");
            return -1;
        }

        //ENCODED MODE
        if (c1 > 0)
        {
            uint8_t color = (uint8_t)c2;

            for (int i = 0; i < c1; i++)
            {
                if (curX < width)
                    row[curX] = color;
                curX++;
            }
        }
        else //ESCAPE CODES
        {
             

            //END OF LINE: 00 00
            if (c2 == 0)        //c2 = 0 → End of Line
            {
                curY++; // Nächste BMP-Zeile
                return 0; // Eine LCD-Zeile fertig
            }

            //END OF BITMAP: 00 01
            else if (c2 == 1)       //c2 = 1 → End of Bitmap
            {
                ended = 1;
                return 0;
            }

            //DELTA MODE: 00 02 dx dy
            else if (c2 == 2)       //c2 = 2 → Delta (dx, dy)
            {
                int dx = nextChar();
                int dy = nextChar();

                if (dx == EOF || dy == EOF)
                {
                    rleError("Fehler: RLE Delta EOF");
                    return -1;
                }

                curX += dx;
                curY += dy;

                if (curX >= width) curX = width - 1; //Seitliche Begrenzung
                if (curY >= height) //Wenn Delta die letzte Zeile überschreitet → Bitmap zu Ende
                {
                    ended = 1;
                    return 0;
                }
            }

            //ABSOLUTE MODE: 00 NN <NN Pixel>
            else
            {
                int count = c2;
                int padding = (count & 1); // Padding bei ungerader Anzahl

                for (int i = 0; i < count; i++)
                {
                    int val = nextChar();
                    if (val == EOF)
                    {
                        rleError("Fehler: ABS EOF");
                        return -1;
                    }

                    if (curX < width)
                        row[curX] = (uint8_t)val;

                    curX++;
                }

                if (padding) //Padding-Byte verwerfen
                    nextChar();
            }
        }

        //Wenn row[] voll ist → nächste Zeile beginnen
        if (curX >= width)
        {
            curY++;
            return 0;
        }
    }

    return 0;
}
