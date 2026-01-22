/**
  * @file mathe.h
  * @author Abdullah Al-Hakimi & Julius Sawilski
  * @date Jan 2026
  * @brief Header file for temperature conversion
  */

#ifndef MATHE_H
#define MATHE_H

#include "sensorDS18B20.h"
#include "sensorDS18S20.h"

/*
****************************************************************************************
* @brief      This function calculates the temperature in Celsius for DS18B20 sensors
*
* @param      data         Pointer to the array of DS18B20 sensor structures
* @param      deviceCount  Number of sensors to process
*
* @return     int          Status code indicating success or error
****************************************************************************************/
int tempCalcCelciusDS18B20(ThermometerDS18B20 *data, int deviceCount);

/*
****************************************************************************************
* @brief      This function calculates the temperature in Celsius for a DS18S20 sensor
*
* @param      data         Pointer to the DS18S20 sensor structure
*
* @return     int          Status code indicating success or error
****************************************************************************************/
int tempCalcCelciusDS18S20(ThermometerDS18S20 *data);

#endif