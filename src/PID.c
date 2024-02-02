#include "PID.h"

//PID calculation
void calculateDutyCycle(pid* pid, int8_t aimTemperature, float tempVal){
    if(pid->coolOrHeatFlag){
        pid->currentError = aimTemperature - tempVal;
    }else{
        pid->currentError = tempVal - aimTemperature;
    }
    //Check if intergral error between min and max duty cycle value
    if(((K_I * pid->integralError <= TIM3_ARR) && pid->currentError >= 0) || 
        ((K_I * pid->integralError >= 0) && pid->currentError < 0)){
            pid->integralError += pid->currentError * dT;
    }
    pid->diffError = (pid->currentError - pid->previousError)/dT;    
    pid->finalDutyCycle = K_P * pid->currentError + K_I * pid->integralError + K_E * pid->diffError;
    pid->previousError = pid->currentError;
}