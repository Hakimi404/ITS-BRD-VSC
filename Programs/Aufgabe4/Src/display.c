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

#define LCD_FONTHEIGHT 16
#define LCD_LASTROW ((320 / LCD_FONTHEIGHT) - 1)
#define LCD_LINESTART 1
#define LCD_LINEEND 41

#define ERR_CODE_YSTART (LCD_LASTROW - ERR_ROWS)
#define ERR_MSG_YSTART (LCD_LASTROW - (ERR_ROWS - 1))
#define ERR_ROWS 4
#define ERR_PRETEXT_LEN 9
#define ERR_CODE_MAXLEN (LCD_LINEEND - ERR_PRETEXT_LEN)
#define ERR_MSG_MAXLEN (LCD_LINEEND * (ERR_ROWS - 1))

struct SensorData {
  char id[MAX_ID_STR_SIZE];
  char temperature[MAX_TEMP_STR_SIZE];
};

struct SensorData valuesToDisplay[MAX_SUPPORTET_DEVICES];
struct SensorData prevValuesToDisplay[MAX_SUPPORTET_DEVICES];
char currentErrorCode[ERR_CODE_MAXLEN];
char currentErrorMsg[ERR_MSG_MAXLEN];

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

  snprintf(currentErrorCode, sizeof(currentErrorCode), "Error:   %s", errName);
  lcdGotoXY(LCD_LINESTART, LCD_LASTROW - ERR_ROWS); 
  lcdPrintlnS(currentErrorCode);

  snprintf(currentErrorMsg, sizeof(currentErrorMsg), "Message: %s", msg);
  lcdPrintS(currentErrorMsg);
}

void clearError() {
  int i = LCD_LINESTART;
  bool strEndRead = false;
  while((i < ERR_CODE_MAXLEN) && (!strEndRead)) {
    if (currentErrorCode[(i - 1)] != ' ') {
      lcdGotoXY(i, ERR_CODE_YSTART);
      lcdPrintC(' ');
    }
    i++;
  }
  strEndRead = false;
  i = LCD_LINESTART;
  while((i < ERR_MSG_MAXLEN) && (!strEndRead)) {
    if (currentErrorMsg[(i - 1)] != ' ') {
      lcdGotoXY((i % LCD_LINEEND), (ERR_MSG_YSTART + (i / LCD_LINEEND)));
      lcdPrintC(' ');
    }
    i++;
  }
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