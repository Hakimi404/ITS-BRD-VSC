/**
  * @file onewire.h
  * @author Abdullah Al-Hakimi & Julius Sawilski
  * @date Jan 2026
  * @brief Header file of the onewire Module
  */

#ifndef ONEWIRE_H
#define ONEWIRE_H

#include <stdint.h>

/*
****************************************************************************************
* @brief      This function resets the OneWire bus and checks for device presence
*
* @return     int          Status code indicating presence of a device or error
****************************************************************************************/
int oneWireReset();

/*
****************************************************************************************
* @brief      This function writes a single bit value to the OneWire bus
*
* @param      bit          The bit value (0 or 1) to be transmitted
*
* @return     int          Status code indicating success or error
****************************************************************************************/
int oneWireWriteBit(int bit);

/*
****************************************************************************************
* @brief      This function reads a single bit value from the OneWire bus
*
* @param      int          Pointer to an integer where the read bit will be stored
*
* @return     int          Status code indicating success or error
****************************************************************************************/
int oneWireReadBit(int *bit);

/*
****************************************************************************************
* @brief      This function writes a full byte (8 bits) to the OneWire bus
*
* @param      uint8_t      byte to be transmitted
*
* @return     int          Status code indicating success or error
****************************************************************************************/
int oneWireWriteByte(uint8_t byte);

/*
****************************************************************************************
* @brief      This function reads a full byte (8 bits) from the OneWire bus
*
* @param      uint8_t       Pointer to a bytesized variable where the read byte is stored
*
* @return     int          Status code indicating success or error
****************************************************************************************/
int oneWireReadByte(uint8_t *byte);

#endif