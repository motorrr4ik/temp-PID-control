#include <stdio.h>
#ifndef PID
#define PID
//pid regulator structure and functions declaration
typedef struct{ 
    int8_t kp;
    int8_t ki;
    int8_t kd;
    float currentError;
    float previousError;
    float integralError;
    float diffError;
    float dutyCycle;
}pid;

void calculateDutyCycle(pid*, int8_t, float);
#endif;