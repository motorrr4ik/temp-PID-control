#include "tempData.h"
void calculateCoolOrHeatFlag(tempData* data, int8_t* previousTempValue){
    if(*previousTempValue < (data->tempVal)){
        data->coolOrHeatFlag = 1;
    }else{
        data->coolOrHeatFlag = 0;
    }
}