#ifndef ONEWIRE_H
#define ONEWIRE_H

#include <stdint.h>
int oneWireReset();
int oneWireWriteBit(int bit);
int oneWireReadBit(int *bit);
int oneWireWriteByte(uint8_t byte);
int oneWireReadByte(uint8_t *byte);

#endif