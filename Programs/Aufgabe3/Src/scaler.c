#include "scaler.h"
#include "lcd_output.h"
#include <math.h>     // Wichtig für floorf, ceilf
#include <stdint.h>   // Wichtig für uint8_t, uint16_t
#include <stddef.h>   // Für NULL
 
void scale_line_box_fit(uint16_t *outBuf, uint8_t **inputRows, int rowCount,
                        int srcW, float scale, int offsetX, int displayImageWidth,
                        RGBQUAD *pal)
{
    //Loop über alle 480 Pixel der LCD-Zeile
    for (int destX = 0; destX < 480; destX++)
    {
        //Prüfen, ob der Pixel im sichtbaren Bereich liegt. Alles außerhalb (links/rechts) = Letterboxing → Schwarz
        if (destX >= offsetX && destX < (offsetX + displayImageWidth))
        {
            //Inverse Mapping: destX → srcX (Position im Originalbild)
            // (destX - offsetX) ist die Koordinate relativ zum Bildstart links
            float srcX_float = (float)(destX - offsetX) / scale;
           
            int srcX_start = (int)floorf(srcX_float);
           
            //Box-Breite
            int boxWidth = (int)ceilf(1.0f / scale);
            if (boxWidth < 1) boxWidth = 1;
 
            int srcX_end = srcX_start + boxWidth;
 
            //Gültige Grenzen sicherstellen
            if (srcX_end > srcW) srcX_end = srcW;
            if (srcX_start >= srcW) srcX_start = srcW - 1;
            if (srcX_start < 0) srcX_start = 0;
 
            // Box-Averaging, Wir mitteln RGB über alle Pixel im Box-Bereich
            uint32_t rSum = 0;
            uint32_t gSum = 0;
            uint32_t bSum = 0;
            int pixelCount = 0;
 
            for (int r = 0; r < rowCount; r++)
            {
                uint8_t *currentRow = inputRows[r];
                // Falls Pointer ungültig ist, überspringen
                if (!currentRow) continue;
 
                for (int sx = srcX_start; sx < srcX_end; sx++)
                {
                    uint8_t idx = currentRow[sx];
                    RGBQUAD c = pal[idx];
                   
                    rSum += c.rgbRed;
                    gSum += c.rgbGreen;
                    bSum += c.rgbBlue;
                    pixelCount++;
                }
            }
 
            //Mittelwert berechnen → Anti-Aliasing
            if (pixelCount > 0)
            {
                RGBQUAD avg;
                avg.rgbRed   = (uint8_t)(rSum / pixelCount);
                avg.rgbGreen = (uint8_t)(gSum / pixelCount);
                avg.rgbBlue  = (uint8_t)(bSum / pixelCount);
                avg.rgbReserved = 0;
 
                outBuf[destX] = rgb_to_16(avg); //16-Bit Format fürs Display
            }
            else
            {
                outBuf[destX] = 0; // Schwarz
            }
        }
        else
        {
            //Außerhalb des Bildes -> Schwarz (Letterboxing Balken)
            outBuf[destX] = 0;
        }
    }
}