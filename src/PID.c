#include "PID.h"
#include "utils.h"
#include "packages.h"
//PID calculation
void calculateDutyCycle(pid* pid, workingPackage *workPack){
    pid->currentError = workPack->currentAimTemperature - workPack->currentTemperature;
    if(pid->currentError > 0){
        workPack->coolOrHeatFlag = 1;
    }else{
        workPack->coolOrHeatFlag = 0;
        pid->currentError = fabs(pid->currentError);
    }

    if(pid->currentError > workPack->pidBorder){
        pid->dutyCycle = TIM3_ARR;
    }else{
        //Check if intergral error between min and max duty cycle value
        if(((pid->ki * pid->integralError <= TIM3_ARR) && pid->currentError >= 0) || 
            ((pid->ki * pid->integralError >= 0) && pid->currentError < 0)){
                pid->integralError += pid->currentError * dT;
        }
        pid->diffError = (pid->currentError - pid->previousError)/dT;    
        pid->dutyCycle = pid->kp * pid->currentError + pid->ki * pid->integralError + pid->kd * pid->diffError;
        pid->previousError = pid->currentError;
    }
}