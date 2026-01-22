/**
  * @file sensorDS18B20.h
  * @author Abdullah Al-Hakimi & Julius Sawilski
  * @date Jan 2026
  * @brief Header file for DS18B20 sensor structures and specific functions.
  */

#ifndef SENSORDS18B20_H
#define SENSORDS18B20_H

#include <stdint.h>

/*
****************************************************************************************
* @brief      Structure representing the raw scratchpad memory layout of a DS18B20
****************************************************************************************/
typedef struct sensorDS18B20
{
    int16_t temperature; 
    int8_t th;
    int8_t tl;
    int8_t config;
    int8_t reserved1;
    int8_t reserved2;
    int8_t reserved3;
    int8_t crc; 
} __attribute__((__packed__)) SensorDS18B20RawData;

/*
****************************************************************************************
* @brief      Structure aggregating the ROM ID, raw data, and calculated temperature
****************************************************************************************/
typedef struct temp {
    uint64_t romID; 
    SensorDS18B20RawData raw;
    float valueCelcius;
}__attribute__((__packed__)) ThermometerDS18B20;

/*
****************************************************************************************
* @brief      This function reads the unique ROM ID from a single connected sensor
*
* @return     int          Status code indicating success or error
****************************************************************************************/
int sensorDS18B20ReadRom(void);

/*
****************************************************************************************
* @brief      This function retrieves temperature measurements for a list of known sensors
*
* @param      sensors      Pointer to the array of DS18B20 sensor structures
* @param      sensorCount  The number of sensors in the array to read from
*
* @return     int          Status code indicating success or error
****************************************************************************************/
int sensorDS18B20GetTemperature(ThermometerDS18B20* sensors, int sensorCount);

/*
****************************************************************************************
* @brief      This function searches the bus for connected DS18B20 sensors
*
* @param      sensors      Pointer to an array where found sensors will be stored
* @param      sensorCount  Pointer to an integer to update with the number of found sensors
*
* @return     int          Status code indicating success or error
****************************************************************************************/
int sensorDS18B20SearchRom(ThermometerDS18B20 *sensors, int *sensorCount);

#endif