#include "stm32f446xx.h"
#include "utils.h"

volatile float finaDutyCycle = 0;    //Global variable for calculate duty cycle

//Temperature calculation variables
float tempCoeffs[4] = {9.418, -40.978, 95.315, -28.127}; //Coeffs for third-order equation for calculating temp value
volatile uint32_t ADCVal   = 0;
volatile float voltageVal  = 0;
volatile float tempVal     = 0;

//PID calculation variables
volatile float currentError      = 0;
volatile float previousError     = 0;
volatile float integralError     = 0;
volatile float diffError         = 0;

void RCCInit(){
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;     //ADC1 activation
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;    //GPIOA ports activation
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;     //TIM3 activation
    RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;     //TIM4 activation
}

void GPIOAInit(){
    GPIOA->MODER |= GPIO_MODER_MODE0;   //Enable analog mode for PA0 pin
    GPIOA->MODER |= GPIO_MODER_MODE6_1; //Enable alternate function mode for PA6 
    GPIOA->MODER |= GPIO_MODER_MODE1_0; //Enable general purpose output mode for PA1
    GPIOA->AFR[0]|= GPIO_AFRL_AFRL6_1;  //Enable alternate function register for PA6 pin, TIM3 CH1 PWM mode
    GPIOA->BSRR  |= GPIO_BSRR_BS1;      //Set PA1 output to high
}

void TIM3Init(){
    TIM3->CR1 |= TIM_CR1_CEN;           //Enable TIM3
    TIM3->PSC = TIM3_PSC - 1;           //Set TIM3 prescalser: look into utils.h file
    TIM3->ARR = TIM3_ARR - 1;           //Set TIM3 arr: check out utils.h file for values
    TIM3->CNT = 0;                      //Set TIM3 counter 
    TIM3->CCMR1 |= TIM_CCMR1_OC1M_1     //Enable PWM generation on TIM3 Channel1 
                 | TIM_CCMR1_OC1M_2;
    TIM3->CCER |= TIM_CCER_CC1E;        //Enable capture/compare register
    TIM3->CCR1 = TIM3_ARR/2 - 1;        //Capture/compare value (informally - duty cycle) set to 50%. Changes between 0 and TIM3_ARR    
}

void TIM4Init(){
    TIM4->CR1 |= TIM_CR1_CEN;   //Enable TIM4
    TIM4->PSC |= TIM4_PSC - 1;  //Set TIM4 prescaler
    TIM4->ARR |= TIM4_ARR - 1;  //Set TIM4 arr: check out utils.h file for values
    TIM4->CNT = 0;              //Set TIM4 counter
    TIM4->DIER = TIM_DIER_UIE;  //Enable interrupt
    NVIC_EnableIRQ(TIM4_IRQn);
}

void ADC1Init(){
    ADC1->CR2 |= ADC_CR2_ADON;      //Enable A/D converter
    ADC1->CR2 |= ADC_CR2_CONT;      //Enable continious conversion
    ADC1->CR2 |= ADC_CR2_SWSTART;   //Start conversion of regular channels
}

//Check if calculated duty cycle value is in bounds, otherwise sets bound value
void checkDutyCycleLimits(float* dutyCycle){
    if(*dutyCycle < TIM3_ARR * 0.4){
        *dutyCycle = TIM3_ARR * 0.4;
    }
    if(*dutyCycle > TIM3_ARR){
        *dutyCycle = TIM3_ARR;
    }
}

//This function checks current temperature status. If upper bound is achieved Peltier is switched to cooling and vise versa
int isTempBoundAchieved(float* temperature){
    //If aim temperature is achieved switch off heating Peltier element, otherwise enable
    if(tempVal > AIM_TEMP_UPPER){
        GPIOA->BSRR  |= GPIO_BSRR_BR1;      //Set PA1 output to low
        return 1;
    }else{
        GPIOA->BSRR  |= GPIO_BSRR_BS1;      //Set PA1 output to high
        return 0;
    }
    return 0;
}

  int main(){
    //Peripheral initialization
    RCCInit();
    GPIOAInit();
    TIM3Init();
    TIM4Init();
    ADC1Init();

    for(;;){
        ADCVal = ADC1->DR;
        voltageVal = REF_VOLTAGE*ADCVal/MAX_ADC_VALUE;
        tempVal = powf(voltageVal,3)*tempCoeffs[0] + powf(voltageVal, 2)*tempCoeffs[1]+voltageVal*tempCoeffs[2]+tempCoeffs[3];
    }
    return 0;
}

void TIM4_IRQHandler(){
    TIM4->SR &= ~TIM_SR_UIF;    //Reset interrupt flag
    if(isTempBoundAchieved(&tempVal)){
        currentError = tempVal - AIM_TEMP_LOWER;
    }else{
        currentError = AIM_TEMP_UPPER - tempVal;
    }
    //Check if intergral error between min and max duty cycle value
    if(((K_I * integralError <= TIM3_ARR) && currentError >= 0) || 
        ((K_I * integralError >= 0) && currentError < 0)){
            integralError += currentError * dT;
    }
    diffError = (currentError - previousError)/dT;    
    finaDutyCycle = K_P * currentError + K_I * integralError + K_E * diffError;

    checkDutyCycleLimits(&finaDutyCycle);
    TIM3->CCR1 = finaDutyCycle; //Update duty cycle
    previousError = currentError;
}