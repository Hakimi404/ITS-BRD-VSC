/**
  * @file display.h
  * @author Abdullah Al-Hakimi & Julius Sawilski
  * @date Jan 2026
  * @brief Header file of display module. 
  */
#ifndef DISPLAY_H
#define DISPLAY_H

#include "sensorDS18B20.h"
#include <stdbool.h>
#include <stddef.h>

/*
 ****************************************************************************************
 * @brief      This function initializes the display to display temperature sensor data
 *
 * @return     void
 ****************************************************************************************/
void initDisplay();

/*
 ****************************************************************************************
 * @brief      This function draws a brief description of each sensor without temperature
 *
 * @param      sensorName   Name of the Sensors used
 * @param      sensors      Pointer containing representations of the connected sensors
 * @param      sensorCount  Count of currently connected Sensors
 *
 * @return     void
 ****************************************************************************************/
void drawAllInfo(char *sensorName, ThermometerDS18B20 *sensors, int sensorCount);

/*
 ****************************************************************************************
 * @brief       This function draws a brief description of a single sensor without temperature
 *
 * @param      sensorName   Name of the Sensors
 * @param      ID           Id of the sensor
 * @param      sensorNumber Determines where the sensor is placed on the display
 *
 * @return     void
 ****************************************************************************************/
void drawInfo(char* sensorName, char *ID, int sensorNumber);

/*
 ****************************************************************************************
 * @brief      This function displays an error message and code on the screen
 *
 * @param      ErrorCode    String representing the specific error code
 * @param      msg          Message describing the cause of the error
 *
 * @return     void
 ****************************************************************************************/
void writeToError(char* ErrorCode, char* msg);

/*
 ****************************************************************************************
 * @brief      This function clears the last error displayed onscreen
 *
 * @return     void
 ****************************************************************************************/
void clearError();

/*
 ****************************************************************************************
 * @brief      This function clears a known string from the area in which it is displayed
 *
 * @param      blancCh          The character used to overwrite/clear the area (z.B ' ')
 * @param      contentsStart_y  The y-Coordinate where the String was written
 * @param      RowContents      Pointer to the previously displayed String
 * @param      rowContentsSize  The size of the String to be cleared
 *
 * @return     void
 ****************************************************************************************/
void clearContents(char blancCh, int contentsStart_y, const char* RowContents, size_t rowContentsSize);

/*
 ****************************************************************************************
 * @brief      This function updates and draws the measured temperature from the DS18B20 sensors
 *
 * @param      sensorCount  The number of sensors to update
 * @param      tempReaded   Pointer to a boolean indicating success of the read operation
 *
 * @return     void
 ****************************************************************************************/
void drawTemperatureDS18B20(int sensorCount, bool *tempReaded);

/*
 ****************************************************************************************
 * @brief      This function converts the sensors measured temperatures into display-ready strings
 *
 * @param      data         Pointer to the array of ThermometerDS18B20 structures
 * @param      sensorCount  Number of sensors to process
 *
 * @return     void
 ****************************************************************************************/
void convertAllValuesToDisplay(ThermometerDS18B20 *data, int sensorCount);

#endif