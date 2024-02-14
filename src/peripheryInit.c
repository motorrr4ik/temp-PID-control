#include "peripheryInit.h"
#include "stm32f446xx.h"
#include "utils.h"
void RCCInit(void){
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;     //ADC1 activation
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;    //GPIOA ports activation
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;     //TIM3 activation
    RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;     //TIM4 activation
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;   //USART2 activation
    RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;     //DMA1 activation
}

void GPIOAInit(void){
    GPIOA->MODER |= GPIO_MODER_MODE0;   //Set PA0 to analog mode
    GPIOA->MODER |= GPIO_MODER_MODE6_1; //Set PA6 to alternate function mode
    GPIOA->MODER |= GPIO_MODER_MODE1_0; //Set PA1 to general purpose output mode
    GPIOA->AFR[0]|= GPIO_AFRL_AFRL6_1;  //Enable alternate function register for PA6 pin, TIM3 CH1 PWM mode
    // GPIOA->BSRR  |= GPIO_BSRR_BS1;      //Set PA1 output to high
    GPIOA->MODER |= GPIO_MODER_MODE2_1; //Set PA2 to alternate function mode
    GPIOA->MODER |= GPIO_MODER_MODE3_1; //Set PA3 to alternate function mode
    GPIOA->AFR[0]|= GPIO_AFRL_AFRL2_0 
                   |GPIO_AFRL_AFRL2_1
                   |GPIO_AFRL_AFRL2_2;  //Enable alternate function register for PA2 pin, USART2 TX
    GPIOA->AFR[0]|= GPIO_AFRL_AFRL3_0 
                   |GPIO_AFRL_AFRL3_1
                   |GPIO_AFRL_AFRL3_2;  //Enable alternate function register for PA3 pin, USART2 RX
}

void USART2Init(void){
    USART2->CR1 |= USART_CR1_UE;            //USART enable
    USART2->CR1 |= USART_CR1_RE;            //USART recieve enable
    USART2->CR1 |= USART_CR1_TE;            //USART transmit enable
    USART2->CR1 |= USART_CR1_IDLEIE;
    USART2->CR3 |= USART_CR3_DMAR;          //USART DMA reading enable
    USART2->CR3 |= USART_CR3_DMAT;          //USART DMA transmitting enable
    USART2->BRR = SYSTEM_CORE_CLOCK/9600;   //Set USART speed   
    NVIC_EnableIRQ(USART2_IRQn);
}

void DMA1Init(uint32_t *varAdressIn, uint32_t sizeIn, uint32_t *varAdressOut, uint32_t sizeOut){
    DMA1_Stream6->PAR = (uint32_t)&USART2->DR;
    DMA1_Stream6->M0AR= varAdressOut;
    DMA1_Stream6->NDTR= sizeOut;
    DMA1_Stream6->CR = 0x4 << DMA_SxCR_CHSEL_Pos;
    DMA1_Stream6->CR |= DMA_SxCR_MINC;
    DMA1_Stream6->CR |= DMA_SxCR_DIR_0;
    DMA1_Stream6->CR |= DMA_SxCR_CIRC;
    DMA1_Stream6->CR |= DMA_SxCR_EN;

    DMA1_Stream5->PAR = (uint32_t)&USART2->DR;
    DMA1_Stream5->M0AR= varAdressIn;
    DMA1_Stream5->NDTR= sizeIn;
    DMA1_Stream5->CR = 0x4 << DMA_SxCR_CHSEL_Pos;
    DMA1_Stream5->CR |= DMA_SxCR_MINC;
    DMA1_Stream5->CR |= DMA_SxCR_CIRC;
    DMA1_Stream5->CR |= DMA_SxCR_EN;
}

void TIM3Init(void){
    TIM3->CR1 |= TIM_CR1_CEN;           //Enable TIM3
    TIM3->PSC = TIM3_PSC - 1;           //Set TIM3 prescalser: check out utils.h file for values
    TIM3->ARR = TIM3_ARR - 1;           //Set TIM3 arr: check out utils.h file for values
    TIM3->CNT = 0;                      //Set TIM3 counter 
    TIM3->CCMR1 |= TIM_CCMR1_OC1M_1     //Enable PWM generation on TIM3 Channel1 
                 | TIM_CCMR1_OC1M_2;
    TIM3->CCER |= TIM_CCER_CC1E;        //Enable capture/compare register
    TIM3->CCR1 = TIM3_ARR/2 - 1;        //Capture/compare value (informally - duty cycle) set to 50%. Changes between 0 and TIM3_ARR    
}

void TIM4Init(void){
    TIM4->CR1 |= TIM_CR1_CEN;   //Enable TIM4
    TIM4->PSC |= TIM4_PSC - 1;  //Set TIM4 prescaler
    TIM4->ARR |= TIM4_ARR - 1;  //Set TIM4 arr: check out utils.h file for values
    TIM4->CNT = 0;              //Set TIM4 counter
    TIM4->DIER = TIM_DIER_UIE;  //Enable interrupt
    NVIC_EnableIRQ(TIM4_IRQn);
}

void ADC1Init(void){
    ADC1->CR2 |= ADC_CR2_ADON;      //Enable A/D converter
    ADC1->CR2 |= ADC_CR2_CONT;      //Enable continious conversion
    ADC1->CR2 |= ADC_CR2_SWSTART;   //Start conversion of regular channels
}