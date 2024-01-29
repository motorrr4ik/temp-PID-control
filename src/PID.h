#ifndef PID
#define PID
#include <stdint.h>
#include <stdlib.h>

typedef struct{ 
    float currentError;
    float previousError;
    float integralError;
    float diffError;
    float finalDutyCycle;
}pid;

void calculateDutyCycle(pid*, int8_t, float);
#endif