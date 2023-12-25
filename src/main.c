#include "stm32f446xx.h"
#include "utils.h"

volatile float finaDutyCycle = 0;    //Global variable for calculate duty cycle
volatile uint8_t millisCounter = 0;  //Count millis for PID

void RCCInit(){
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;     //ADC1 activation
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;    //GPIOA ports activation
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;     //TIM3 activation
    RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;     //TIM4 activation
}

void GPIOAInit(){
    GPIOA->MODER |= GPIO_MODER_MODE0;   //Enable analog mode for PA0 pin
    GPIOA->MODER |= GPIO_MODER_MODE6_1; //Enable alternate function mode for PA6 
    GPIOA->AFR[0]|= GPIO_AFRL_AFRL6_1;  //Enable alternate function register for PA6 pin, TIM3 CH1 PWM mode
    GPIOA->MODER |= GPIO_MODER_MODE1_0; //Enable general purpose output mode PA1
    GPIOA->BSRR  |= GPIO_BSRR_BS1;      //Set PA1 output to high
}

void TIM3Init(){
    TIM3->CR1 |= TIM_CR1_CEN;           //Enable TIM3
    TIM3->PSC = TIM3_PSC - 1;           //Set TIM3 prescalser: look into utils.h file
    TIM3->ARR = TIM3_ARR - 1;           //Set TIM3 arr: look into utils.h file
    TIM3->CNT = 0;                      //Set TIM3 counter 
    TIM3->CCMR1 |= TIM_CCMR1_OC1M_1     //Enable PWM generation on TIM3 Channel1 
                 | TIM_CCMR1_OC1M_2;
    TIM3->CCER |= TIM_CCER_CC1E;        //Enable capture/compare register
    TIM3->CCR1 = TIM3_ARR/2 - 1;        //Capture/compare value (informally - duty cycle) set to 50%    
    // TIM3->DIER |= TIM_DIER_UIE;         //TIM3 interrupt enable
    // NVIC_EnableIRQ(TIM3_IRQn);
}

void TIM4Init(){
    TIM4->CR1 |= TIM_CR1_CEN;
    TIM4->PSC |= TIM4_PSC - 1;
    TIM4->ARR |= TIM4_ARR - 1;
    TIM4->CNT = 0;
    TIM4->DIER = TIM_DIER_UIE;
}

void ADC1Init(){
    ADC1->CR2 |= ADC_CR2_ADON;      //Enable A/D converter
    ADC1->CR2 |= ADC_CR2_CONT;      //Enable continious conversion
    ADC1->CR2 |= ADC_CR2_SWSTART;   //Start conversion of regular channels
}

void checkDutyCycleLimits(float* dutyCycle){
    if(*dutyCycle < 0){
        *dutyCycle = 0;
    }
    if(*dutyCycle > TIM3_ARR){
        *dutyCycle = TIM3_ARR;
    }
}

int main(){
    //Peripheral initialization
    RCCInit();
    GPIOAInit();
    TIM3Init();
    ADC1Init();

    //Temperature calculation variables
    float tempCoeffs[4] = {9.418, -40.978, 95.315, -28.127}; //Coeffs for third-order equation for calculating temp value
    volatile uint32_t val      = 0;
    volatile float voltageVal  = 0;
    volatile float tempVal     = 0;

    //PID calculation variables
    float currentError      = 0;
    float previousError     = 0;
    float integralError     = 0;
    float diffError         = 0;
    float secondsCounter    = 0;

    for(;;){
        val = ADC1->DR;
        voltageVal = REF_VOLTAGE*val/MAX_ADC_VALUE;
        tempVal = powf(voltageVal,3)*tempCoeffs[0] + powf(voltageVal, 2)*tempCoeffs[1]+voltageVal*tempCoeffs[2]+tempCoeffs[3];
    
        secondsCounter = millisCounter/1000;
        currentError = AIM_TEMP - tempVal;

        //Check if intergral error between min and max duty cycle value
        if(((K_I * integralError <= TIM3_ARR) && currentError >= 0) || 
            ((K_I * integralError >= 0) && currentError < 0)){
                integralError += currentError * secondsCounter;
        }
        diffError = (currentError - previousError)/secondsCounter;
        finaDutyCycle = K_P * currentError + K_I * integralError + K_E * diffError;

        checkDutyCycleLimits(&finaDutyCycle);
        previousError = currentError;
        millisCounter = 0;
    
    }
    return 0;
}

void TIM3_IRQHandler(){
    TIM3->SR &= ~TIM_SR_UIF;    //Reset interrupt flag
    TIM3->CCR1 = finaDutyCycle; //Update duty cycle
}

void TIM4_IRQHandler(){

}