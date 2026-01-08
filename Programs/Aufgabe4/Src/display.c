#include "display.h"
#include "LCD_GUI.h"
#include "global.h"
#include "lcd.h"
#include "sensorDS18B20.h"
#include "stdio.h" 
#include <string.h>
#define MAX_TEMP_STR_SIZE 9
#define MAX_ID_STR_SIZE 18

char valuesToDisplay[MAX_SUPPORTET_DEVICES][MAX_TEMP_STR_SIZE];
char prevValuesToDisplay[MAX_SUPPORTET_DEVICES][MAX_TEMP_STR_SIZE];

void initDisplay() {
  lcdGotoXY(0, 0);
  lcdPrintS("Sensor");
  lcdGotoXY(9, 0);
  lcdPrintS("PDROM");
  lcdGotoXY(30, 0);
  lcdPrintS("Temp. [C]");
};

void drawInfo(char *sensorName, uint64_t ID, int sensorNumber) {
  int y = 2 * sensorNumber;
  char textID[MAX_ID_STR_SIZE];
  lcdGotoXY(0, y);
  lcdPrintS(sensorName);
  lcdGotoXY(9, y);
  snprintf(textID, MAX_ID_STR_SIZE, "0x%llX", ID);
  lcdPrintS(textID);
}
void convertAllValuesToDisplay(ThermometerDS18B20 *data, int sensorCount) {
  for (int i = 0; i < sensorCount; i++) {
    snprintf(valuesToDisplay[i], MAX_TEMP_STR_SIZE, "%.5f", data[i].valueCelcius);
  }
}

void drawTemperatureDS18B20(int sensorCount, bool *tempReaded) {
  static int currentPos = 0;
  static int x = 30;
  int y = 2;
  for (int i = 0; i < sensorCount; i++) {
    if (currentPos < strlen(valuesToDisplay[i]) && valuesToDisplay[i][currentPos] != prevValuesToDisplay[i][currentPos]) {
      char toDraw = valuesToDisplay[i][currentPos];
      lcdGotoXY(x + currentPos, y * (i + 1));
      lcdPrintC(toDraw);
      prevValuesToDisplay[i][currentPos] = toDraw;
    }
  }
  currentPos++;
  if (currentPos == MAX_TEMP_STR_SIZE) {
    *tempReaded = false;
    currentPos = 0;
  }
}