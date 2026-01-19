#include "display.h"
#include "LCD_GUI.h"
#include "LCD_general.h"
#include "global.h"
#include "lcd.h"
#include "sensorDS18B20.h"
#include "stdio.h" 
#include <string.h>
#define MAX_TEMP_STR_SIZE 9
#define MAX_ID_STR_SIZE 20
#define EMTPYID "                  "
#define ERROR_ROWS 2
#define STARTFONTHEIGHT 16
#define STARTFONTWIDTH 11
#define ERR_CLEARROW "           "

struct SensorData {
  char id[MAX_ID_STR_SIZE];
  char temperature[MAX_TEMP_STR_SIZE];
};

struct SensorData valuesToDisplay[MAX_SUPPORTET_DEVICES];
struct SensorData prevValuesToDisplay[MAX_SUPPORTET_DEVICES];

void initDisplay() {
  lcdGotoXY(0, 0);
  lcdPrintS("Sensor");
  lcdGotoXY(9, 0);
  lcdPrintS("PDROM");
  lcdGotoXY(30, 0);
  lcdPrintS("Temp. [C]");
};
void drawAllInfo(char *sensorName, ThermometerDS18B20 *sensors, int sensorCount) {
  char textID[MAX_ID_STR_SIZE];

  for (int i = 0; i < MAX_SUPPORTET_DEVICES; i++) {
    if (i < sensorCount) {
      snprintf(textID, MAX_ID_STR_SIZE, "0x%llX", sensors[i].romID);
      if (strcmp(textID, prevValuesToDisplay[i].id) != 0) {
        drawInfo("DS18B20", textID, i + 1);
        strcpy(prevValuesToDisplay[i].id, textID);
        strcpy(valuesToDisplay[i].id, textID);
      }
    }
    else if (valuesToDisplay[i].id[0] != '\0') {
      lcdGotoXY(0, 2 * (i + 1));
      lcdPrintS(EMTPYID);
      valuesToDisplay[i].id[0] = '\0';
      prevValuesToDisplay[i].id[0] = '\0';
    }
  }
}
void drawInfo(char *sensorName, char* ID, int sensorNumber) {
  int y = 2 * sensorNumber;
  lcdGotoXY(0, y);
  lcdPrintS(sensorName);
  lcdGotoXY(9, y);
  lcdPrintS(ID);
}

void writeToError(char *errName, char *msg) {
  lcdGotoXY(0, (STARTFONTHEIGHT - 1) - ERROR_ROWS); // TODO: Prüfen ob Fontsize geändert werden könnte und an Änderung anpassen
  lcdPrintlnS("Error: %s"); // TODO: Ein leerzeichen zu viel und snprinf für richtige Formatstrings
  lcdPrintlnS("Deskription: %s");
}

void clearError() {
  lcdGotoXY(0, (STARTFONTHEIGHT - 1) - ERROR_ROWS); // TODO: Prüfen ob Fontsize geändert werden könnte und an Änderung anpassen
  lcdPrintlnS(ERR_CLEARROW);
  lcdPrintlnS(ERR_CLEARROW);
}

void convertAllValuesToDisplay(ThermometerDS18B20 *data, int sensorCount) {
  for (int i = 0; i < sensorCount; i++) {
    snprintf(valuesToDisplay[i].temperature, MAX_TEMP_STR_SIZE, "%.5f", data[i].valueCelcius);
  }
}

void drawTemperatureDS18B20(int sensorCount, bool *tempReaded) {
  static int currentPos = 0;
  static int x = 30;
  int y = 2;
  for (int i = 0; i < sensorCount; i++) {
    bool charDifferent = (valuesToDisplay[i].temperature[currentPos] != prevValuesToDisplay[i].temperature[currentPos]);
    if ((currentPos < strlen(valuesToDisplay[i].temperature)) && charDifferent) {
      char toDraw = valuesToDisplay[i].temperature[currentPos];
      lcdGotoXY(x + currentPos, y * (i + 1));
      lcdPrintC(toDraw);
      prevValuesToDisplay[i].temperature[currentPos] = toDraw;
    }
  }
  currentPos++;
  if (currentPos == MAX_TEMP_STR_SIZE) {
    *tempReaded = false;
    currentPos = 0;
  }
}