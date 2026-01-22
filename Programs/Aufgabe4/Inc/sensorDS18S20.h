/**
  * @file sensorDS18S20.h
  * @author Abdullah Al-Hakimi & Julius Sawilski
  * @date Jan 2026
  * @brief Header file for DS18S20 sensor structures and specific functions.
  */

#ifndef SENSORDS18S20_H
#define SENSORDS18S20_H

#include <stdint.h>

/*
****************************************************************************************
* @brief      Structure representing the raw scratchpad memory layout of a DS18S20
****************************************************************************************/
typedef struct sensorDS18S20
{
    int16_t temperature; 
    int8_t th;
    int8_t tl;
    int8_t config;
    int8_t reserved;
    int8_t countRemain;
    int8_t countPerC;
    int8_t crc; 
} __attribute__((__packed__)) SensorDS18S20RawData;

/*
****************************************************************************************
* @brief      Structure aggregating the ROM ID, raw data, and calculated temperature
****************************************************************************************/
typedef struct tempDS18S20 {
    uint64_t romID; 
    SensorDS18S20RawData raw;
    float valueCelcius;
}__attribute__((__packed__)) ThermometerDS18S20;

/*
****************************************************************************************
* @brief      This function reads the unique ROM ID from a single connected sensor
*
* @return     int          Status code indicating success or error
****************************************************************************************/
int sensorDS18S20ReadRom();

/*
****************************************************************************************
* @brief      This function retrieves temperature readings from the DS18S20 sensor
*
* @return     int          Status code indicating success or error
****************************************************************************************/
int sensorDS18S20GetTemperature();

#endif