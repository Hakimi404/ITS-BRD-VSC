#include "bmp_reader.h"
#include "headers.h"
#include "input.h"
#include "errorhandler.h"
#include "LCD_GUI.h"
#include "fontsFLASH.h"
#include <stdio.h>

/**
 * Wenn =1, wird die RLE-Dekodierung erzwungen,
 * auch wenn der BMP-Header "BI_RGB" meldet.
 * → In diesem Projekt NICHT benutzt.
 */
#define FORCE_RLE_DECODE 0

//Statische Decoder-Zustände für ein geöffnetes BMP
static int g_width = 0;
static int g_compression = 0;
static int g_bitCount = 0;
static bool g_rle_eof = false;

//Reset aller globalen Zustände vor Einlesen eines neuen Bildes
void bmp_reset(void)
{
    g_width = 0;
    g_compression = 0;
    g_bitCount = 0;
    g_rle_eof = false;
}

/* bmp_start()
 *   Liest Header + Palette + springt zum Pixeloffset
 */
int bmp_start(BITMAPFILEHEADER *fh, BITMAPINFOHEADER *ih, RGBQUAD *pal)
{
    bmp_reset();
    char debugBuf[64];

    //1) BMP-Header einlesen (nutzt intern COMread → liest 512-Byte-Chunks)
    if (readHeaders() != EOK) {
        lcdErrorMsg("Header Read Error");
        return NOK;
    }

    getFileHeader(fh);
    getInfoHeader(ih);

    g_width       = ih->biWidth;
    g_compression = ih->biCompression;
    g_bitCount    = ih->biBitCount;

    
    // FEHLER 1: Nur 8 Bit BMP unterstützt
   
    if (g_bitCount != 8)
    {
        lcdErrorMsg("Fehler: Nur 8-Bit BMP!");
        return NOK;
    }

    
    // FEHLER 2: Nur RLE8 (BI_RLE8) erlaubt
    
    if (g_compression != BI_RLE8)
    {
        lcdErrorMsg("Fehler: Nicht RLE8 komprimiert!");
        return NOK;
    }

    
    //Debug-Ausgabe für Entwicklung --> Wird für jedes Bild einmal oben links angezeigt
    
    GUI_clear(BLACK);
    snprintf(debugBuf, sizeof(debugBuf),
             "W:%d H:%d C:%d B:%d",
             (int)ih->biWidth, (int)ih->biHeight,
             (int)ih->biCompression, (int)ih->biBitCount);

    Coordinate pos = {0, 0};
    GUI_disStr(pos, debugBuf, &Font16, WHITE, RED);

    
    //Farbpalette lesen (256 Einträge zu je 4 Bytes: B,G,R,Res)
    
    int colors = ih->biClrUsed;
    if (colors == 0) colors = 256;

    for (int i = 0; i < colors; i++)
    {
        int b   = nextChar();
        int g   = nextChar();
        int r   = nextChar();
        int res = nextChar();

        if (b == EOF || g == EOF || r == EOF)
        {
            lcdErrorMsg("EOF in Palette");
            return NOK;
        }

        pal[i].rgbBlue     = (uint8_t)b;
        pal[i].rgbGreen    = (uint8_t)g;
        pal[i].rgbRed      = (uint8_t)r;
        pal[i].rgbReserved = (uint8_t)res;
    }

    
    //Zum Datenoffset springen (bfOffBits) --> Alles davor sind Header + Palette + evtl. zusätzliche Metadaten
    
    int currentBytesRead = 14 + 40 + (colors * 4);
    int targetOffset     = fh->bfOffBits;
    int gap              = targetOffset - currentBytesRead;

    if (gap < 0)
    {
        lcdErrorMsg("Fehler: Negatives Gap");
        return NOK;
    }
        //Bytes überspringen
    for (int i = 0; i < gap; i++)
    {
        if (nextChar() == EOF)
        {
            lcdErrorMsg("EOF in Gap");
            return NOK;
        }
    }

    return EOK;
}

//read_row_rle()  Dekodiert EINE Ausgabezeile aus dem RLE8-Datenstrom
static int read_row_rle(uint8_t *row, int width)
{
    if (g_rle_eof)
        return 0; // Bitmap ist komplett fertig

    int x = 0;

    while (1)
    {
        int b1 = nextChar();
        int b2 = nextChar();

        if (b1 == EOF || b2 == EOF)
            return -1;

        // ENCODED MODE (b1 > 0)  / b1 = Anzahl wiederholungen , b2 = Farbindex
        if (b1 > 0)
        {
            int count = b1;
            int colorIndex = b2;

            for (int i = 0; i < count; i++)
            {
                if (x < width)
                    row[x] = (uint8_t)colorIndex;
                x++;
            }
        }
        else
        {
            // ESCAPE MODE (b1 == 0)
            if (b2 == 0)
                return 0; // 0,0 → End Of Line

            if (b2 == 1) //0,1 → End Of Bitmap
            {
                g_rle_eof = true;
                return 0;
            }

            if (b2 == 2) //0,2,dx,dy → Cursor verschieben
            {
                int dx = nextChar();
                int dy = nextChar();

                x += dx;
                // dy wird ignoriert (Bild wird Bottom-Up gelesen)
            }
            else
            {
                // ABSOLUTE MODE
                int count = b2;

                for (int i = 0; i < count; i++)
                {
                    int val = nextChar();
                    if (val == EOF) return -1;

                    if (x < width)
                        row[x] = (uint8_t)val;
                    x++;
                }

                // Padding (auf Wortgrenze)
                if (count % 2 != 0)
                    nextChar();
            }
        }
    }
}


// RAW READER (falls nicht RLE) — wird nicht benutzt, aber bleibt drin

static int read_row_raw(uint8_t *row, int width)
{
    for (int i = 0; i < width; i++)
    {
        int c = nextChar();
        if (c == EOF)
            return -1;

        row[i] = (uint8_t)c;
    }

    int padding = (4 - (width % 4)) % 4;

    for (int i = 0; i < padding; i++)
    {
        if (nextChar() == EOF)
            return -1;
    }

    return 0;
}


//Hauptfunktion zum Lesen einer Zeile  → entscheidet zwischen RAW und RLE8

int bmp_read_row(uint8_t *row, int width)
{
    if (g_compression == BI_RLE8)
        return read_row_rle(row, width);

    return read_row_raw(row, width);
}
