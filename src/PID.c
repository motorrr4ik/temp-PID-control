#include "PID.h"
#include "utils.h"
#include "packages.h"
//PID calculation
void calculateDutyCycle(pid* pid, workingPackage *workPack){
    heatingCell currentCell;
    //iterate through every cell and calculate params
    for(int8_t i = 0; i < HEATING_CELL_NUMBER; ++i){
        currentCell = workPack->cells[i];
        pid->currentError = currentCell.aimTemp - currentCell.currentTemp;
        if(!currentCell.coolOrHeatFlag){
            pid->currentError = 0 - pid->currentError;
        }
    // pid->currentError = workPack->currentAimTemperature - workPack->currentTemperature;
    // if(!workPack->coolOrHeatFlag){
    //     pid->currentError = 0 - pid->currentError;
    // }
    // if(pid->currentError > 0){
    //     workPack->coolOrHeatFlag = 1;
    // }else{
    //     workPack->coolOrHeatFlag = 0;
    //     pid->currentError = fabs(pid->currentError);
    // }
        if(pid->currentError > workPack->pidBorder){
            currentCell.dutyCycle = TIM3_ARR;
        }else{
            //Check if intergral error between min and max duty cycle value
            if(((pid->ki * pid->integralError <= TIM3_ARR) && pid->currentError >= 0) || 
                ((pid->ki * pid->integralError >= 0) && pid->currentError < 0)){
                    pid->integralError += pid->currentError/20;
            }
            pid->diffError = (pid->currentError - pid->previousError)/dT;    
            currentCell.dutyCycle = pid->kp * pid->currentError + pid->ki * pid->integralError + pid->kd * pid->diffError;
            pid->previousError = pid->currentError;
        }
    }
}

void setWorkMode(pid* pid, workingPackage* workPack){
    heatingCell currentCell;
    for(int8_t i = 0; i < HEATING_CELL_NUMBER; ++i){
        currentCell = workPack->cells[i];
        pid->currentError = currentCell.aimTemp - currentCell.currentTemp;
        if(pid->currentError > 0){
            currentCell.coolOrHeatFlag = 1;
        }else{
            currentCell.coolOrHeatFlag = 0;
        }  
    }
    // pid->currentError = workPack->currentAimTemperature - workPack->currentTemperature;
    // if(pid->currentError > 0){
    //     workPack->coolOrHeatFlag = 1;
    // }else{
    //     workPack->coolOrHeatFlag = 0;
    // }  
}