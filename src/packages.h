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
    float currentTemperature;
}workingPackage;
#endif