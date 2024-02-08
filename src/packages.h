#include <stdio.h>
#ifndef PACKAGES
#define PACKAGES
//structs and datatypes
typedef struct{
    int8_t temp[3];
}outputPackage;

typedef struct{
    int8_t enable;
    int8_t cycles;
    int8_t pidBorder;
    int8_t temps[3];
    int8_t times[3];
    int8_t coeffs[3];
}inputPackage;

typedef struct{
    int8_t coolOrHeatFlag;
    int8_t currentCycle; 
    int8_t currentAimTemperature;
    int8_t pidBorder;
    int8_t currentPeriod;
    float currentTemperature;
    float temperatureEquationCoeffs[4];
}workingPackage;
#endif