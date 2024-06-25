#ifndef PERIPHERY_H
#define PERIPHERY_H

#include "stand_workflow.h"
#include "stm32f4xx.h"

//-Common Periph func-
void InitPeriphery(stand_workflow_t *stand);
void EnablePeriphery(void);
void DisablePeriphery(void);
void RCCSetUp(void);
//-----GPIO setup-----
void GPIOASetup(void);
void GPIOBSetup(void);
void GPIOCSetup(void);
//-----TIMs setup-----
void TIM1SetUp(void);
void TIM3SetUp(void);
void TIM4SetUp(void);
void TIM4_IRQHandler(void);
//----USART setup-----
void USART2SetUp(void);
void USART2_IRQHandler(void);
//----DMA setup-------
void DMA1SetUp(uint32_t input_data_struct, uint32_t output_data_struct);
void DMA2SetUp(uint32_t input_adc_data_struct, uint32_t input_spi_adc_data_struct);
void ADC1SetUp(void);
void SPI1SetUp(void);
#endif