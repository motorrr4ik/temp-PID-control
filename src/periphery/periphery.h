#ifndef PERIPHERY_H
#define PERIPHERY_H

#include "stm32f4xx.h"

void RCCSetUp(void);
//-----GPIO setup-----
void GPIOASetup(void);
void GPIOBSetup(void);
void GPIOCSetup(void);
//-----TIMs setup-----
void TIM1SetUp(void);
void TIM3SetUp(void);
//----USART setup-----
void USART2SetUp(void);
void USART2_IRQn(void);
//----DMA setup-------
void DMA1SetUp(void);
void DMA2SetUp(void);
void ADC1SetUp(void);
void SPI1SetUp(void);
#endif