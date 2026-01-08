#ifndef SENSORDS18B20_H
#define SENSORDS18B20_H

#include <stdint.h>

typedef struct sensorDS18B20
{
    int16_t temperature; 
    int8_t th;
    int8_t tl;
    int8_t config;
    int8_t reserved1;
    int8_t reserved2;
    int8_t reserved3;
    int8_t crc; 
} __attribute__((__packed__)) SensorDS18B20RawData;

typedef struct temp {
    uint64_t romID; 
    SensorDS18B20RawData raw;
    float valueCelcius;
}__attribute__((__packed__)) ThermometerDS18B20;

int sensorDS18B20ReadRom();
int sensorDS18B20GetTemperature();
int sensorDS18B20ReadRom();
int sensorDS18B20SearchRom();

#endif