#ifndef BMP_READER_H
#define BMP_READER_H
 
#include <stdint.h>
#include "BMP_types.h"
 
// Liest Header und Palette, setzt interne Status-Variablen
int bmp_start(BITMAPFILEHEADER *fh, BITMAPINFOHEADER *ih, RGBQUAD *pal);
 
// Liest GENAU eine Zeile (dekomprimiert RLE oder entfernt Padding automatisch)
int bmp_read_row(uint8_t *row, int width);
 
// Setzt den RLE-Decoder zurück (wichtig bei neuem Bild)
void bmp_reset(void);
 
#endif