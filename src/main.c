#include "stm32f446xx.h"
#include "utils.h"
#include <stdlib.h>

volatile float finaDutyCycle = 0;    //Global variable for calculate duty cycle

//Temperature calculation variables
float tempCoeffs[4] = {9.418, -40.978, 95.315, -28.127}; //Coeffs for third-order equation for calculating temp value
volatile uint32_t ADCVal   = 0;
volatile float voltageVal  = 0;
volatile float tempVal     = 0;

//Transfer variables
volatile int8_t tempTransfer = 0;
volatile uint8_t dmaMessage[3] = {0};
volatile uint8_t messageSize = 0;

//PID calculation variables
volatile float currentError      = 0;
volatile float previousError     = 0;
volatile float integralError     = 0;
volatile float diffError         = 0;

//Logical variables
int8_t peltierHeatCoolFlag = 0;
int8_t aimTemperature = AIM_TEMP_UPPER;

void RCCInit(){
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;     //ADC1 activation
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;    //GPIOA ports activation
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;     //TIM3 activation
    RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;     //TIM4 activation
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;   //USART2 activation
    RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;     //DMA1 activation
}

void GPIOAInit(){
    GPIOA->MODER |= GPIO_MODER_MODE0;   //Set PA0 to analog mode
    GPIOA->MODER |= GPIO_MODER_MODE6_1; //Set PA6 to alternate function mode
    GPIOA->MODER |= GPIO_MODER_MODE1_0; //Set PA1 to general purpose output mode
    GPIOA->AFR[0]|= GPIO_AFRL_AFRL6_1;  //Enable alternate function register for PA6 pin, TIM3 CH1 PWM mode
    GPIOA->BSRR  |= GPIO_BSRR_BS1;      //Set PA1 output to high
    GPIOA->MODER |= GPIO_MODER_MODE2_1; //Set PA2 to alternate function mode
    GPIOA->MODER |= GPIO_MODER_MODE3_1; //Set PA3 to alternate function mode
    GPIOA->AFR[0]|= GPIO_AFRL_AFRL2_0 
                   |GPIO_AFRL_AFRL2_1
                   |GPIO_AFRL_AFRL2_2;  //Enable alternate function register for PA2 pin, USART2 TX
    GPIOA->AFR[0]|= GPIO_AFRL_AFRL3_0 
                   |GPIO_AFRL_AFRL3_1
                   |GPIO_AFRL_AFRL3_2;  //Enable alternate function register for PA3 pin, USART2 RX
}

void USART2Init(){
    USART2->CR1 |= USART_CR1_UE;            //USART enable
    USART2->CR1 |= USART_CR1_RE;            //USART recieve enable
    USART2->CR1 |= USART_CR1_TE;            //USART transmit enable
    USART2->CR3 |= USART_CR3_DMAR;          //USART DMA reading enable
    USART2->CR3 |= USART_CR3_DMAT;          //USART DMA transmitting enable
    USART2->BRR = SYSTEM_CORE_CLOCK/9600;  //Set USART speed   
}

void DMA1Init(){
    DMA1_Stream6->PAR = (uint32_t)&USART2->DR;
    DMA1_Stream6->M0AR= &dmaMessage;
    DMA1_Stream6->NDTR= sizeof(dmaMessage);
    DMA1_Stream6->CR = 0x4 << DMA_SxCR_CHSEL_Pos;
    DMA1_Stream6->CR |= DMA_SxCR_MINC;
    DMA1_Stream6->CR |= DMA_SxCR_DIR_0;
}

void TIM3Init(){
    TIM3->CR1 |= TIM_CR1_CEN;           //Enable TIM3
    TIM3->PSC = TIM3_PSC - 1;           //Set TIM3 prescalser: check out utils.h file for values
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

//Transmitting temperuter value via USART2. Temporary solution
void transmitTemperatureValue(){
    DMA1->HIFCR = DMA_HIFCR_CTCIF6 | DMA_HIFCR_CHTIF6;
    DMA1_Stream6->CR |= DMA_SxCR_EN;
    // while(!(DMA1_Stream6->NDTR == 0));
    // DMA1_Stream6->CR &= ~DMA_SxCR_EN;
    // DMA1_Stream6->NDTR= sizeof(tempVal);
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

//Function wich toggles PA1 pin to switch Peltier mode to heat/cool. 0 - cooling, 1 - heating status flags
void switchPeltier(){
    if(peltierHeatCoolFlag){                //Cooling mode
        GPIOA->BSRR  &= ~GPIO_BSRR_BS1;     //Switch PA1 to low mode
        GPIOA->BSRR  |= GPIO_BSRR_BR1;
        aimTemperature = AIM_TEMP_LOWER;    //Set lower temp bound for cooling
        peltierHeatCoolFlag = 0;            //Set local flag to cooling mode
    }else{                                  //Heating mode
        GPIOA->BSRR  &= ~GPIO_BSRR_BR1;     //Switch PA1 to high mode
        GPIOA->BSRR  |= GPIO_BSRR_BS1; 
        aimTemperature = AIM_TEMP_UPPER;    //Set upper temp bound for cooling
        peltierHeatCoolFlag = 1;            //Set local flag to heating mode
    }
}

int main(){
    //Peripheral initialization
    RCCInit();
    GPIOAInit();
    USART2Init();
    DMA1Init();
    TIM3Init();
    TIM4Init();
    ADC1Init();

    for(;;){
        ADCVal = ADC1->DR;
        voltageVal = REF_VOLTAGE*ADCVal/MAX_ADC_VALUE;
        tempVal = powf(voltageVal,3)*tempCoeffs[0] + powf(voltageVal, 2)*tempCoeffs[1]+voltageVal*tempCoeffs[2]+tempCoeffs[3];
        tempTransfer = sizeof(dmaMessage);
        messageSize = sprintf(dmaMessage, "%d\n", (int8_t)tempVal);
    }
    return 0;
}

void TIM4_IRQHandler(){
    TIM4->SR &= ~TIM_SR_UIF;    //Reset interrupt flag
    transmitTemperatureValue();
    //Condition to check wheter aim temp is achieved
    if(currentError <= TEMP_DELTA){
        switchPeltier();
    }
    currentError = abs(aimTemperature - tempVal);
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