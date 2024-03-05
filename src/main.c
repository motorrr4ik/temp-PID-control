#include "stm32f446xx.h"
#include "peripheryInit.h" 
#include "utils.h"
#include "tempData.h"
#include "PID.h"
#include "packages.h"

//01 03 05 37 21 2A 19 19 19 23 32 00

volatile uint32_t secondsCounter = 0;
volatile inputPackage inPack = {0};
volatile outputPackage outPack = {0};
volatile workingPackage workPack = {0, .temperatureEquationCoeffs = {9.418, -40.978, 95.315, -28.127}};
volatile pid regulator = {0};

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

//Check if calculated duty cycle value is in bounds, otherwise sets bound value
void checkDutyCycleLimits(float* dutyCycle){
    if(*dutyCycle < 0){
        *dutyCycle = 0;
    }
    if(*dutyCycle > TIM3_ARR){
        *dutyCycle = TIM3_ARR;
    }
}

void disablePeltier(void){
    GPIOA->BSRR  &= ~GPIO_BSRR_BS1;   //Switch PA1 to low mode
    GPIOA->BSRR  &= ~GPIO_BSRR_BR1;   //Switch PA1 to high mode
}

void updateCycleParameters(void){
    workPack.currentCycle++;
    if(workPack.currentCycle >= inPack.cycles){
        inPack.enable = 0;
        disablePeltier();
    }else{
        workPack.currentAimTemperature = inPack.temps[workPack.currentCycle];
        workPack.currentPeriod = inPack.times[workPack.currentCycle];
        regulator.currentError = 1;
    }
}

//Function wich toggles PA1 pin to switch Peltier mode to heat/cool. 0 - cooling, 1 - heating status flags
void switchPeltier(void){
    if(!workPack.coolOrHeatFlag){          //Cooling mode
        GPIOA->BSRR  &= ~GPIO_BSRR_BS1;   //Switch PA1 to low mode
        GPIOA->BSRR  |= GPIO_BSRR_BR1;
        regulator.integralError = 0;      //Reset integral error
    }else{                                //Heating mode
        GPIOA->BSRR  &= ~GPIO_BSRR_BR1;   //Switch PA1 to high mode
        GPIOA->BSRR  |= GPIO_BSRR_BS1; 
        regulator.integralError = 0;      //Reset integral error
    }
}

//If aim temperature achieved, start counting then switch temperature mode
void countPeriod(void){
    if(regulator.currentError < TEMP_DELTA){
        secondsCounter = 0;
        while (secondsCounter < workPack.currentPeriod){
            calculateTemperature(workPack.temperatureEquationCoeffs, sizeof(workPack.temperatureEquationCoeffs)/sizeof(workPack.temperatureEquationCoeffs[0]));
        };
        updateCycleParameters();
        setWorkMode(&regulator, &workPack);
        switchPeltier();
    }
}

void peltierControlManager(void){
    TIM3->CCR1 = regulator.dutyCycle;
    countPeriod();
}

int main(){
    //Peripheral initialization
    RCCInit();
    GPIOAInit();
    USART2Init();
    DMA1Init(&inPack, sizeof(inPack), &outPack,sizeof(outPack));
    TIM3Init();
    TIM4Init();
    ADC1Init();
    SysTick_Config(SYSTEM_CORE_CLOCK);
    for(;;){
        calculateTemperature(workPack.temperatureEquationCoeffs, sizeof(workPack.temperatureEquationCoeffs)/sizeof(workPack.temperatureEquationCoeffs[0]));
        if(inPack.enable){
            peltierControlManager();
        }
    }
    return 0;
}

void TIM4_IRQHandler(void){
    TIM4->SR &= ~TIM_SR_UIF;    //Reset interrupt flag
    calculateDutyCycle(&regulator, &workPack);
    checkDutyCycleLimits(&(regulator.dutyCycle));
}

void USART2_IRQHandler(void){
    if(USART2->SR & USART_SR_IDLE){
        (void)USART2->DR;
        workPack.currentCycle = 0;
        workPack.currentAimTemperature = inPack.temps[workPack.currentCycle];
        workPack.pidBorder = inPack.pidBorder;
        workPack.currentPeriod = inPack.times[workPack.currentCycle];
        regulator.kp = inPack.coeffs[0];
        regulator.ki = inPack.coeffs[1];
        regulator.kd = inPack.coeffs[2];
    }
    setWorkMode(&regulator, &workPack);
    switchPeltier();
}
void SysTick_Handler(void){
    if(secondsCounter==60){ 
        secondsCounter = 0;
    }
    ++secondsCounter;
}