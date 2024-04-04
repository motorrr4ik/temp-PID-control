#include "peripheryInit.h"
#include "stm32f446xx.h"
#include "utils.h"
void RCCInit(void){
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;     //ADC1 activation
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;    //GPIOA ports activation
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;    //GPIOB ports activation
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;    //GPIOC ports activation
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;     //TIM3 activation
    RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;     //TIM4 activation
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;   //USART2 activation
    RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;     //DMA1 activation
    RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;     //DMA2 activation
}

void GPIOAInit(void){
//-----------------------------GPIOA setting up-----------------------------
//                     Setting alternate functions mode
    GPIOA->MODER |= GPIO_MODER_MODE2_1; //Set PA2 to alternate function mode
    GPIOA->MODER |= GPIO_MODER_MODE3_1; //Set PA3 to alternate function mode
    GPIOA->MODER |= GPIO_MODER_MODE5_1; //Set PA5 to alternate function mode
    GPIOA->MODER |= GPIO_MODER_MODE6_1; //Set PA6 to alternate function mode
    GPIOA->MODER |= GPIO_MODER_MODE7_1; //Set PA7 to alternate function mode
    GPIOA->MODER |= GPIO_MODER_MODE8_1; //Set PA8 to alternate function mode
    GPIOA->MODER |= GPIO_MODER_MODE9_1; //Set PA9 to alternate function mode
    GPIOA->MODER |= GPIO_MODER_MODE11_1;//Set PA11 to alternate function mode
//Enable alternate function register for PA2 pin, USART2 TX
    GPIOA->AFR[0]|= GPIO_AFRL_AFRL2_0 
                   |GPIO_AFRL_AFRL2_1
                   |GPIO_AFRL_AFRL2_2;
//Enable alternate function register for PA3 pin, USART2 RX
    GPIOA->AFR[0]|= GPIO_AFRL_AFRL3_0 
                   |GPIO_AFRL_AFRL3_1
                   |GPIO_AFRL_AFRL3_2; 
//Enable alternate function register for PA5 pin, SPI1 CLK
    GPIOA->AFR[0]|= GPIO_AFRL_AFRL5_0 
                   |GPIO_AFRL_AFRL5_2;
//Enable alternate function register for PA6 pin, SPI1 MISO
    GPIOA->AFR[0]|= GPIO_AFRL_AFRL6_0 
                   |GPIO_AFRL_AFRL6_2;
//Enable alternate function register for PA7 pin, SPI1 MOSI
    GPIOA->AFR[0]|= GPIO_AFRL_AFRL7_0 
                   |GPIO_AFRL_AFRL7_2;
//Enable alternate function register for PA8 pin, PWM1_1
    GPIOA->AFR[1]|= GPIO_AFRH_AFRH0_0;
//Enable alternate function register for PA9 pin, PWM1_2
    GPIOA->AFR[1]|= GPIO_AFRH_AFRH1_0;
//Enable alternate function register for PA11 pin, PWM1_4
    GPIOA->AFR[1]|= GPIO_AFRH_AFRH3_0;

//                          Setting analog mode
    GPIOA->MODER |= GPIO_MODER_MODE0_1
                | GPIO_MODER_MODE0_0;
    GPIOA->MODER |= GPIO_MODER_MODE1_1
                | GPIO_MODER_MODE1_0;

//                        Setting general IO mode
    GPIOA->MODER |= GPIO_MODER_MODE10_0; //Set PA10 to general purpose output mode
    GPIOA->MODER |= GPIO_MODER_MODE12_0; //Set PA12 to general purpose output mode

}

void GPIOBInit(void){
//-----------------------------GPIOB setting up-----------------------------
//                     Setting alternate functions mode
    GPIOB->MODER |= GPIO_MODER_MODE5_1; //Set PB5 to alternate function mode
    GPIOB->MODER |= GPIO_MODER_MODE15_1;//Set PB15 to alternate function mode
//Enable alternate function register for PB5 pin, PWM3_2
    GPIOB->AFR[0]|= GPIO_AFRL_AFRL5_1;
//Enable alternate function register for PB15 pin, PWM1_3
    GPIOB->AFR[1]|= GPIO_AFRH_AFRH7_1;
//                          Setting analog mode
    GPIOB->MODER |= GPIO_MODER_MODE1_1
                | GPIO_MODER_MODE1_0;
//                        Setting general IO mode
    GPIOB->MODER |= GPIO_MODER_MODE2_0;
    GPIOB->MODER |= GPIO_MODER_MODE3_0;
    GPIOB->MODER |= GPIO_MODER_MODE4_0;
    GPIOB->MODER |= GPIO_MODER_MODE6_0;
    GPIOB->MODER |= GPIO_MODER_MODE10_0;
    GPIOB->MODER |= GPIO_MODER_MODE12_0;
    GPIOB->MODER |= GPIO_MODER_MODE14_0;
}

void GPIOCInit(void){
//-----------------------------GPIOC setting up-----------------------------
//                     Setting alternate functions mode
    GPIOC->MODER |= GPIO_MODER_MODE6_1; //Set PA6 to alternate function mode
    GPIOC->MODER |= GPIO_MODER_MODE8_1; //Set PA8 to alternate function mode
    GPIOC->MODER |= GPIO_MODER_MODE9_1; //Set PA9 to alternate function mode
//Enable alternate function register for PC6 pin, PWM3_1 
    GPIOC->AFR[0]|= GPIO_AFRL_AFRL6_1;
//Enable alternate function register for PC8 pin, PWM3_3 
    GPIOC->AFR[1]|= GPIO_AFRH_AFRH0_1;
//Enable alternate function register for PC9 pin, PWM3_4 
    GPIOC->AFR[1]|= GPIO_AFRH_AFRH1_1;
//                          Setting analog mode 
    GPIOC->MODER |= GPIO_MODER_MODE0_1
                | GPIO_MODER_MODE0_0;
    GPIOC->MODER |= GPIO_MODER_MODE1_1
                | GPIO_MODER_MODE1_0;
    GPIOC->MODER |= GPIO_MODER_MODE2_1
                | GPIO_MODER_MODE2_0;
    GPIOC->MODER |= GPIO_MODER_MODE3_1
                | GPIO_MODER_MODE3_0; 
    GPIOC->MODER |= GPIO_MODER_MODE4_1
                | GPIO_MODER_MODE4_0;  
//                        Setting general IO mode   
    GPIOC->MODER |= GPIO_MODER_MODE7_0;
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

void DMA1Init(uint32_t varAdressIn, uint32_t sizeIn, uint32_t varAdressOut, uint32_t sizeOut){
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

void DMA2Init(uint32_t varAddr, uint32_t varSize){
    DMA2_Stream0->PAR = (uint32_t)&ADC1->DR;
    DMA2_Stream0->M0AR= varAddr;
    DMA2_Stream0->NDTR= varSize;
    DMA2_Stream0->CR |= DMA_SxCR_MINC;
    DMA2_Stream0->CR |= DMA_SxCR_CIRC;
    DMA2_Stream0->CR |= DMA_SxCR_MSIZE_1;
    DMA2_Stream0->CR |= DMA_SxCR_EN;
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
    ADC1->CR1 |= ADC_CR1_SCAN;      //Enable scan mode
    ADC1->SQR1|= ADC_SQR1_L_3;      //Select 8 channels to read from
    // ADC1->SQR3|= ADC_SQR3_SQ1_0;
    ADC1->SQR3|= ADC_SQR3_SQ2_3 | ADC_SQR3_SQ2_0;
    ADC1->SQR3|= ADC_SQR3_SQ3_3 | ADC_SQR3_SQ3_1;
    ADC1->SQR3|= ADC_SQR3_SQ4_3 | ADC_SQR3_SQ4_1 | ADC_SQR3_SQ4_0;
    ADC1->SQR3|= ADC_SQR3_SQ5_3 | ADC_SQR3_SQ5_2;
    ADC1->SQR3|= ADC_SQR3_SQ6_3 | ADC_SQR3_SQ6_2 | ADC_SQR3_SQ6_0;
    ADC1->SQR2|= ADC_SQR2_SQ7_4 | ADC_SQR2_SQ7_2;
    ADC1->SQR2|= ADC_SQR2_SQ8_0;
    ADC1->CR2 |= ADC_CR2_DMA;       //Enable DMA
    ADC1->CR2 |= ADC_CR2_ADON;      //Enable A/D converter
    ADC1->CR2 |= ADC_CR2_CONT;      //Enable continious conversion
    ADC1->CR2 |= ADC_CR2_SWSTART;   //Start conversion of regular channels
}