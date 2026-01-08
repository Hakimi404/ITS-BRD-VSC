#ifndef SENSORDS18S20_H
#define SENSORDS18S20_H

#include <stdint.h>

typedef struct sensorDS18S20
{
    int16_t temperature; 
    int8_t th;
    int8_t tl;
    int8_t config;
    int8_t reserved;
    int8_t countRemain;
    int8_t countPerC;
    int8_t crc; 
} __attribute__((__packed__)) SensorDS18S20RawData;

typedef struct tempDS18S20 {
    uint64_t romID; 
    SensorDS18S20RawData raw;
    float valueCelcius;
}__attribute__((__packed__)) ThermometerDS18S20;

int sensorDS18S20ReadRom();
int sensorDS18S20GetTemperature();

#endif