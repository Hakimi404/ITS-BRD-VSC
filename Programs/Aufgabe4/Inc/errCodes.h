/**
  * @file errCodes.h
  * @author Abdullah Al-Hakimi & Julius Sawilski
  * @date Jan 2026
  * @brief Header file defining error codes 
  */

#ifndef ERRCODES_H
#define ERRCODES_H

/*
****************************************************************************************
* @brief      Operation completed successfully without errors
****************************************************************************************/
#define OK 0

/*
****************************************************************************************
* @brief      Error indicating an unexpected read value
****************************************************************************************/
#define ERR_UNKNOWN_BIT -1

/*
****************************************************************************************
* @brief      Error indicating a CRC validation failure during temperature sensor register evaluation
****************************************************************************************/
#define ERR_BIT_FLIPPED -2

/*
****************************************************************************************
* @brief      Error indicating no sensor was detected on the bus
****************************************************************************************/
#define ERR_NO_SENSOR -3

#endif