#include "tempData.h"
void calculateCoolOrHeatFlag(tempData* data, int8_t* previousTempValue){
    if(*previousTempValue-(data->tempVal) < 0){
        data->coolOrHeatFlag = 0;
    }else{
        data->coolOrHeatFlag = 1;
    }
}