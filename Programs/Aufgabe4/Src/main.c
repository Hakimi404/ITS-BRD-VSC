/**
  ******************************************************************************
  * @file    main.c
  * @author  Franz Korf
  * @brief   Kleines Testprogramm fuer neu erstelle Fonts.
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/

#include "display.h"
#include "stm32f4xx_hal.h"
#include "init.h"
#include "LCD_GUI.h"
#include "LCD_Touch.h"
#include "lcd.h"
#include "fontsFLASH.h"
#include "additionalFonts.h"
#include "error.h"
#include "onewire.h"
#include "sensorDS18B20.h"
#include "errCodes.h"
#include "timer.h"
#include "timing.h"
#include "errorStatus.h"


int main(void) {
	initITSboard();    // Initialisierung des ITS Boards
	
	GUI_init(DEFAULT_BRIGHTNESS);   // Initialisierung des LCD Boards mit Touch
	TP_Init(false);                 // Initialisierung des LCD Boards mit Touch
  initTimer();
  initDisplay();
  int rc = OK;
  rc = sensorDS18B20SearchRom();
	if (rc!=OK) {
      indicateError(rc);
  }
	// Test in Endlosschleife
	while(1) {
		rc = sensorDS18B20GetTemperature();
    if (rc!=OK) {
      indicateError(rc);
  }
	}
}

// EOF