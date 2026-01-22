/**
  * @file ds18x20common.h
  * @author Abdullah Al-Hakimi & Julius Sawilski
  * @date Jan 2026
  * @brief Header file of common DS18x20 sensor functionality.
  */

#ifndef DS18X20COMMON_H
#define DS18X20COMMON_H

#include <stdint.h>
/*
****************************************************************************************
* @brief      This function prepares the sensor for temperatureReading
*
* @return     void
****************************************************************************************/
void sensorFullThrottle();

/*
****************************************************************************************
* @brief      This function selects a specific device on the OneWire bus by its unique ID
*
* @param      targetID     The 64-bit unique identifier of the target sensor
*
* @return     int          Status code indicating success or failure of the selection
****************************************************************************************/
int sensorSelect(uint64_t targetID);

/*
****************************************************************************************
* @brief      This function scans the OneWire bus to identify all connected devices
*
* @param      deviceIDs    Pointer to an array where found device IDs will be stored
* @param      devicesCount Pointer to an integer to store the total count of found devices
*
* @return     int          Status code indicating the result of the scan operation
****************************************************************************************/
int scanOneWireBus(uint64_t *deviceIDs, int *devicesCount);

#endif