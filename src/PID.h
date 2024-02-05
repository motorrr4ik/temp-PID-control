#include "utils.h"
#ifndef PID
#define PID
//pid regulator structure and functions declaration
typedef struct{ 
    int8_t coolOrHeatFlag;
    float currentError;
    float previousError;
    float integralError;
    float diffError;
    float finalDutyCycle;
}pid;

extern void calculateDutyCycle(pid*, int8_t, float);
#endif