#ifndef LCD_OUTPUT_H
#define LCD_OUTPUT_H

#include <stdint.h>
#include "BMP_types.h"

uint16_t rgb_to_16(RGBQUAD c);
void lcd_draw_pixel(int x, int y, uint8_t idx, RGBQUAD *pal);
void lcd_draw_row(int x, int y, uint16_t *buf, int w);

#endif
