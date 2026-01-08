#ifndef RLE_DECODER_H
#define RLE_DECODER_H

#include <stdint.h>

int rle_decode_row(uint8_t *row, int width);
void rle_reset(void);
void rle_reset_with_height(int h);

#endif
