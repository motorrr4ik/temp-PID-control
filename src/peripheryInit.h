#include <stdio.h>
#ifndef PERIPHERY_INIT
#define PERIPHERY_INIT
//Peripheral functions
void RCCInit(void);
void GPIOAInit(void);
void GPIOBInit(void);
void GPIOCInit(void);
void USART2Init(void);
void DMA1Init(uint32_t, uint32_t, uint32_t, uint32_t);
void DMA2Init(uint32_t, uint32_t);
void TIM2Inint(void);
void TIM3Init(void);
void TIM4Init(void);
void ADC1Init(void);
void SPI1Inint(void);
#endif