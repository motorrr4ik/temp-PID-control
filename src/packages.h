#ifndef PACKAGES
#define PACKAGES
#include <stdio.h>
#include "utils.h"
//structs and datatypes

//data for one heating cell
typedef struct{
    int8_t coolOrHeatFlag;
    int8_t aimTemp;
    float currentTemp;
    float dutyCycle;
    int8_t tempValues[CYCLES_NUMBER];
}heatingCell;

//data from photodiode
typedef struct{         
    uint32_t sensorValue1;
    uint32_t sensorValue2;
    uint32_t sensorValue3;
    uint32_t sensorValue4;
    uint32_t sensorValue5;
    uint32_t sensorValue6;
    uint32_t sensorValue7;
    uint32_t sensorValue8;
}lightSensorData;

//Temperature values received from heating cells
typedef struct{
    uint32_t sensorValue1;
    uint32_t sensorValue2;
    uint32_t sensorValue3;
    uint32_t sensorValue4;
    uint32_t sensorValue5;
    uint32_t sensorValue6;
    uint32_t sensorValue7;
    uint32_t sensorValue8;
}heatingCellsTempData;

typedef struct{
    int8_t temp[3];
}outputPackage;

typedef struct{
    int8_t standOnOff;
    int8_t pidBorder;
    int8_t temps[CYCLES_NUMBER];
    int8_t timePeriod;
    int8_t coeffs[CYCLES_NUMBER];
}inputPackage;

//main process structure
typedef struct{
    int8_t standOnOff;
    heatingCellsTempData cellsTempData;
    heatingCell cells[HEATING_CELL_NUMBER];
    lightSensorData sensorsData;
    int8_t currentCycle; 
    int8_t pidBorder;
    int8_t timePeriod;
    float temperatureEquationCoeffs[4];
}workingPackage;
#endif