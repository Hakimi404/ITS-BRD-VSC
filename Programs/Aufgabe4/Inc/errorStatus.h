/**
  * @file errorStatus.h
  * @author Abdullah Al-Hakimi & Julius Sawilski
  * @date Jan 2026
  * @brief Header file for status handling and error indication 
  */

#ifndef ERRORSTATUS_H
#define ERRORSTATUS_H

/*
****************************************************************************************
* @brief      This function processes an errorCode and indicates the error to the user
*
* @param      rc           ErrorCode to be evaluated
*
* @return     void
****************************************************************************************/
void indicateError(int rc);

#endif