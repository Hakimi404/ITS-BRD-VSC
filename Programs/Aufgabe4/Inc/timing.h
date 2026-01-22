/**
  * @file timing.h
  * @author Abdullah Al-Hakimi & Julius Sawilski
  * @date Jan 2026
  * @brief Header file for timing and delay functions.
  */

#ifndef TIMING_H
#define TIMING_H

#include <stdint.h>

/*
****************************************************************************************
* @brief      This function pauses the program execution for a specified duration
*
* @param      duration     Microseconds to wait
*
* @return     int          Status code indicating success of the operation
****************************************************************************************/
int wait(uint32_t duration);

#endif