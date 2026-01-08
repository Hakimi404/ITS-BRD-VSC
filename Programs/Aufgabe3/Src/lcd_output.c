#include "lcd_output.h"
#include "LCD_GUI.h"

uint16_t rgb_to_16(RGBQUAD c)
{
    int r = c.rgbRed >> 3;
    int g = c.rgbGreen >> 2;
    int b = c.rgbBlue >> 3;
    return (r << 11) | (g << 5) | b;
}

void lcd_draw_pixel(int x, int y, uint8_t idx, RGBQUAD *pal)
{
    uint16_t col = rgb_to_16(pal[idx]);

    Coordinate crd;
    crd.x = x;
    crd.y = y;

    GUI_drawPoint(crd, col, DOT_PIXEL_1X1, DOT_FILL_AROUND);
}

void lcd_draw_row(int x, int y, uint16_t *buf, int w)
{
    Coordinate crd;
    crd.x = x;
    crd.y = y;

    GUI_WriteLine(crd, w, buf);
}