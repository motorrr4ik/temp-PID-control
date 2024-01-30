#ifndef PID
#define PID
#include <stdint.h>
#include <stdlib.h>

//pid regulator structure and functions description
typedef struct{ 
    float currentError;
    float previousError;
    float integralError;
    float diffError;
    float finalDutyCycle;
}pid;

void calculateDutyCycle(pid*, int8_t, float);
#endif