#ifndef SCALER_H
#define SCALER_H
 
#include <stdint.h>
#include "BMP_types.h"
 
/**
 * @brief Skaliert eine Zeile mit Box-Filter und Fit-to-Screen Logik (Letterboxing).
 *
 * @param outBuf            Ausgabepuffer für die LCD-Zeile (480 Pixel, RGB565)
 * @param inputRows         Array der Quellzeilen (Palette-Indizes)
 * @param rowCount          Anzahl der gültigen Quellzeilen
 * @param srcW              Breite des Originalbildes
 * @param scale             Skalierungsfaktor (<= 1.0)
 * @param offsetX           X-Verschiebung für Zentrierung (schwarzer Rand links)
 * @param displayImageWidth Effektive Breite des Bildes auf dem Screen
 * @param pal               Farbpalette
 */
void scale_line_box_fit(uint16_t *outBuf, uint8_t **inputRows, int rowCount,
                        int srcW, float scale, int offsetX, int displayImageWidth,
                        RGBQUAD *pal);
 
#endif