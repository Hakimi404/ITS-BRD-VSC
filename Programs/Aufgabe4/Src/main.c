/**
  ******************************************************************************
  * @file    main.c
  * @author  Franz Korf
  * @brief   Kleines Testprogramm fuer neu erstelle Fonts.
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/

#include "display.h"
#include "init.h"
#include "LCD_GUI.h"
#include "LCD_Touch.h"
#include "sensorDS18B20.h"
#include "errCodes.h"
#include "timer.h"
#include "global.h"
#include "errorStatus.h"

#define NO_OLDSENSORCOUNT -1

static ThermometerDS18B20 sensors[MAX_SUPPORTET_DEVICES];
static int old_sensorCount = NO_OLDSENSORCOUNT;
static int curr_sensorCount = 0;
static int errorOnDisplay = false;

int main(void) {
	initITSboard();    // Initialisierung des ITS Boards
	
	GUI_init(DEFAULT_BRIGHTNESS);   // Initialisierung des LCD Boards mit Touch
	TP_Init(false);                 // Initialisierung des LCD Boards mit Touch
  initTimer();
  initDisplay();
  int rc = OK;
	// Test in Endlosschleife
	while(1) {
    // sensoren suchen und anzeigen
    rc = sensorDS18B20SearchRom(sensors, &curr_sensorCount);
    if ((rc != OK) && (rc != errorOnDisplay)) {
      indicateError(rc);
      errorOnDisplay = rc;
    }
    if (errorOnDisplay != OK && rc == OK) {
      clearError();
      errorOnDisplay = OK;
    }
    if (curr_sensorCount != old_sensorCount) {
      drawAllInfo("DS18B20", sensors, curr_sensorCount);
    }

    //temperaturwerte lesen und ausgeben
    rc = sensorDS18B20GetTemperature(sensors, curr_sensorCount);
		if ((rc != OK) && (rc != errorOnDisplay)) {
      indicateError(rc);
      errorOnDisplay = rc;
    }
    if (errorOnDisplay != OK && rc == OK) {
      clearError();
      errorOnDisplay = OK;
    }
	}
}

// EOF