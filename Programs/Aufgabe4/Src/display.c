#include "display.h"
#include "LCD_GUI.h"
#include "LCD_general.h"
#include "global.h"
#include "lcd.h"
#include "sensorDS18B20.h"
#include "stdio.h" 
#include <stddef.h>
#include <string.h>

#define MAX_TEMP_STR_SIZE 9
#define MAX_ID_STR_SIZE 20
#define TEMP_STARTX 30
#define TEMP_ENDX (TEMP_STARTX + MAX_TEMP_STR_SIZE)
#define ROWSBETWEENSENSORS 2
#define EMTPYID "                  "

#define LCD_FONTHEIGHT 16
#define LCD_LASTROW ((320 / LCD_FONTHEIGHT) - 1)
#define LCD_LINESTART 1
#define LCD_LINEEND 42

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
      //valuesToDisplay[i].temperature[0] = '\0';
      //valuesToDisplay[i].id[0] = '\0';
      memset(prevValuesToDisplay[i].id, 0, sizeof(prevValuesToDisplay[i].id));
      memset(prevValuesToDisplay[i].temperature, 0, sizeof(prevValuesToDisplay[i].temperature));
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
  clearContents(' ', ERR_CODE_YSTART, currentErrorCode, sizeof(currentErrorCode));
  clearContents(' ', ERR_MSG_YSTART, currentErrorMsg, sizeof(currentErrorMsg));
}

void clearContents(char blancCh, int contentsStart_y, const char* rowContents, size_t rowContentsSize) {
  int i = 0;
  bool strEndRead = false;
  while(!strEndRead && (i < rowContentsSize)) {
    if (rowContents[i] != blancCh) {
      lcdGotoXY(((i + LCD_LINESTART) % LCD_LINEEND), (contentsStart_y + ((i + LCD_LINESTART) / LCD_LINEEND)));
      lcdPrintC(blancCh);
    }
    i++;
    strEndRead = ((rowContents[i - 1]) == '\0');
  }
}
 
void convertAllValuesToDisplay(ThermometerDS18B20 *data, int sensorCount) {
  for (int i = 0; i < sensorCount; i++) {
    snprintf(valuesToDisplay[i].temperature, MAX_TEMP_STR_SIZE, "%.5f", data[i].valueCelcius);
  }
}

void drawTemperatureDS18B20(int sensorCount, bool *tempRead) {
  for(int i = 0; i < MAX_TEMP_STR_SIZE; i++) {
    for (int j = 0; j < sensorCount; j++) {
      bool charDifferent = (valuesToDisplay[j].temperature[i] != prevValuesToDisplay[j].temperature[i]);
      if (i < strlen(valuesToDisplay[j].temperature) && charDifferent) {
        char toDraw = valuesToDisplay[j].temperature[i];
        lcdGotoXY((TEMP_STARTX + i), (ROWSBETWEENSENSORS * (j + 1)));
        lcdPrintC(toDraw);
        prevValuesToDisplay[j].temperature[i] = toDraw;
      }
    }
  }
}