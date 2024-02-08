#include <stdio.h>
#ifndef PERIPHERY_INIT
#define PERIPHERY_INIT
//Peripheral macros
void RCCInit(void);
void GPIOAInit(void);
void USART2Init(void);
void DMA1Init(uint32_t*, uint32_t, uint32_t*, uint32_t);
void TIM3Init(void);
void TIM4Init(void);
void ADC1Init(void);
#endif