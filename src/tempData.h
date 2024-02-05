#include "utils.h"
#ifndef TEMP_DATA
#define TEMP_DATA

typedef struct{
    int8_t coolOrHeatFlag;
    int8_t tempVal;
}tempData;

extern void calculateCoolOrHeatFlag(tempData*, int8_t*);
#endif