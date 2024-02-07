#include "PID.h"
#include "utils.h"
//PID calculation
void calculateDutyCycle(pid* pid, int8_t aimTemperature, float tempVal){
    pid->currentError = aimTemperature - tempVal;
    //Check if intergral error between min and max duty cycle value
    if(((pid->ki * pid->integralError <= 1000) && pid->currentError >= 0) || 
        ((pid->ki * pid->integralError >= 0) && pid->currentError < 0)){
            pid->integralError += pid->currentError * dT;
    }
    pid->diffError = (pid->currentError - pid->previousError)/dT;    
    pid->dutyCycle = pid->kp * pid->currentError + pid->ki * pid->integralError + pid->kd * pid->diffError;
    pid->previousError = pid->currentError;
}