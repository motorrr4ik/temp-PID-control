#include "stm32f446xx.h"
#include "peripheryInit.h" 
#include "utils.h"
#include "tempData.h"
#include "PID.h"
#include "packages.h"

// volatile int8_t aimTemperature = 0;
volatile uint32_t secondsCounter = 0;

volatile inputPackage inPack = {0};
volatile outputPackage outPack = {0};
volatile workingPackage workPack = {0};
volatile pid regulator = {1};

// void updateDMA1S5NDTRValue(void){
//     DMA1_Stream5->CR &= ~DMA_SxCR_EN;
//     DMA1->HIFCR = DMA_HIFCR_CTCIF5 | DMA_HIFCR_CHTIF5;
//     DMA1_Stream5->NDTR = sizeof(dmaRecieve);
//     DMA1_Stream5->CR |= DMA_SxCR_EN;
// }

//Init heating for peltier element
// void startProcess(void){
//     if(data->coolOrHeatFlag){
//         GPIOA->BSRR  &= ~GPIO_BSRR_BR1;     //Switch PA1 to high mode
//         GPIOA->BSRR  |= GPIO_BSRR_BS1; 
//         aimTemperature = AIM_TEMP_UPPER;    //Set upper temp bound for cooling
//     }else{
//         GPIOA->BSRR  &= ~GPIO_BSRR_BS1;     //Switch PA1 to low mode
//         GPIOA->BSRR  |= GPIO_BSRR_BR1;
//         aimTemperature = AIM_TEMP_LOWER;    //Set lower temp bound for cooling
//     }
// }

//Calculate current temperature based on ADC value
void calculateTemperature(float *coeffs, int8_t size){
    float arrValue = 0;
    float temperatureValue = 0;
    int8_t j = size - 1;
    uint32_t ADCVal = ADC1->DR;
    float voltageVal = REF_VOLTAGE*ADCVal/MAX_ADC_VALUE;
    for(int8_t i = 0; i < size; ++i, --j){
        arrValue = *(coeffs+i);
        temperatureValue += powf(voltageVal, j)*arrValue;
    }
    workPack.currentTemperature = temperatureValue;
    sprintf(outPack.temp, "%d\n", (int8_t)temperatureValue);
}

// void resetDMAReceivedMessage(void){
//     int8_t length = strlen(dmaRecieve.value);
//     for(int8_t i = 0; i < length; ++i){
//         dmaRecieve.value[i] = 0;
//     }
// }


//Check if calculated duty cycle value is in bounds, otherwise sets bound value
void checkDutyCycleLimits(float* dutyCycle){
    if(*dutyCycle < 0){
        *dutyCycle = 0;
    }
    if(*dutyCycle > TIM3_ARR){
        *dutyCycle = TIM3_ARR;
    }
}

//Function wich toggles PA1 pin to switch Peltier mode to heat/cool. 0 - cooling, 1 - heating status flags
// void switchPeltier(void){
//     if(data->coolOrHeatFlag){           //Cooling mode
//         GPIOA->BSRR  &= ~GPIO_BSRR_BS1;     //Switch PA1 to low mode
//         GPIOA->BSRR  |= GPIO_BSRR_BR1;
//         aimTemperature = AIM_TEMP_LOWER;    //Set lower temp bound for cooling
//         // regulator.integralError = 0;        //Reset integral error
//         // regulator.coolOrHeatFlag = 0;       //Set local flag to cooling mode
//     }else{                                  //Heating mode
//         GPIOA->BSRR  &= ~GPIO_BSRR_BR1;     //Switch PA1 to high mode
//         GPIOA->BSRR  |= GPIO_BSRR_BS1; 
//         aimTemperature = AIM_TEMP_UPPER;    //Set upper temp bound for cooling
//         // regulator.integralError = 0;        //Reset integral error
//         // regulator.coolOrHeatFlag = 1;       //Set local flag to heating mode
//     }
// }

//If aim temperature achieved, start counting then switch temperature mode
// void countPeriod(void){
//     if(regulator.currentError < TEMP_DELTA){
//         secondsCounter = 0;
//         while (secondsCounter < 60){
//             calculateTemperature();
//             transmitTemperatureValue();
//         };
//         resetRegulatorValue();
//         switchPeltier();
//         data++;
//     }
// }

// void peltierControlManager(void){
//     if(abs(aimTemperature - tempVal)>ERROR_BORDER){
//         TIM4->DIER = 0;
//         TIM3->CCR1 = TIM3_ARR;      //Set maximum duty cycle
//     }else{
//         TIM4->DIER = TIM_DIER_UIE;  //Enable interrupt and activate PID calculation
//     }
// }

//Coverts received message from char to int
void fromCharToInt(int8_t *arr, int16_t *res){
    int8_t size = strlen(arr);
    int8_t j = size-1;
    for(int8_t i = 0; i < size; ++i, --j, arr++){
        *arr = *arr-'0';
        *res += (*arr)*pow(10,j);
    }
}

//get number of temperature peaks and allocate memory for temperature points array
// void receiveTemperaturePeaksNumber(void){
//     updateDMA1S5NDTRValue();
//     while(dmaRecieve.value[0] == 0){}   //wait until message is received
//     fromCharToInt(dmaRecieve.value, &temperaturePeaksNumber);
//     data = (tempData*)malloc(temperaturePeaksNumber*sizeof(tempData));
//     resetDMAReceivedMessage();
// }

//reads temperature peaks by their amount 
// void readTemperaturePeaksValues(void){
//     calculateTemperature();
//     int16_t receivedTemp = 0;
//     int8_t prevTemp = tempVal;
//     tempData *dummy = data;
//     for(int8_t i = 0; i < temperaturePeaksNumber; ++i){
//         updateDMA1S5NDTRValue();
//         while(dmaRecieve.value[0] == 0){}
//         fromCharToInt(dmaRecieve.value, &receivedTemp);
//         data->tempVal = receivedTemp;
//         calculateCoolOrHeatFlag(data, &prevTemp);
//         prevTemp = receivedTemp;
//         receivedTemp = 0;
//         resetDMAReceivedMessage();
//         data++;
//     }
//     data = dummy;
// }

int main(){
    //Peripheral initialization
    RCCInit();
    GPIOAInit();
    USART2Init();
    DMA1Init(&inPack, &outPack);
    TIM3Init();
    TIM4Init();
    ADC1Init();
    SysTick_Config(SYSTEM_CORE_CLOCK);
    float tempCoeffs[4] = {9.418, -40.978, 95.315, -28.127}; //Coeffs for third-order equation for calculating temp value
    for(;;){
        calculateTemperature(tempCoeffs, sizeof(tempCoeffs)/sizeof(tempCoeffs[0]));
        if(inPack.enable){
            
        }
        // peltierControlManager();
        // countPeriod();
    }
    return 0;
}

void TIM4_IRQHandler(void){
    TIM4->SR &= ~TIM_SR_UIF;    //Reset interrupt flag
    // calculateDutyCycle(&regulator, aimTemperature, tempVal);
    checkDutyCycleLimits(&(regulator.dutyCycle));
    TIM3->CCR1 = regulator.dutyCycle; //Update duty cycle
}

void SysTick_Handler(void){
    if(secondsCounter==60){ 
        secondsCounter = 0;
    }
    ++secondsCounter;
}