#include "periphery.h"

void DMA1SetUp(uint32_t input_data_struct, uint32_t output_data_struct)
{
    DMA1_Stream6->PAR  = (uint32_t)&USART2->DR;
    DMA1_Stream6->M0AR = output_data_struct;
    DMA1_Stream6->NDTR = sizeof(output_data_struct);
    DMA1_Stream6->CR   = 0x4 << DMA_SxCR_CHSEL_Pos;
    DMA1_Stream6->CR |= DMA_SxCR_MINC;
    DMA1_Stream6->CR |= DMA_SxCR_DIR_0;
    DMA1_Stream6->CR |= DMA_SxCR_CIRC;
    DMA1_Stream6->CR |= DMA_SxCR_EN;

    DMA1_Stream5->PAR  = (uint32_t)&USART2->DR;
    DMA1_Stream5->M0AR = input_data_struct;
    DMA1_Stream5->NDTR = sizeof(input_data_struct);
    DMA1_Stream5->CR   = 0x4 << DMA_SxCR_CHSEL_Pos;
    DMA1_Stream5->CR |= DMA_SxCR_MINC;
    DMA1_Stream5->CR |= DMA_SxCR_CIRC;
    DMA1_Stream5->CR |= DMA_SxCR_EN;
}

void DMA2SetUp(uint32_t input_adc_data_struct, uint32_t input_spi_adc_data_struct)
{
    DMA2_Stream0->PAR  = (uint32_t)&ADC1->DR;
    DMA2_Stream0->M0AR = input_adc_data_struct;
    DMA2_Stream0->NDTR = sizeof(input_adc_data_struct);
    DMA2_Stream0->CR |= DMA_SxCR_MINC;
    DMA2_Stream0->CR |= DMA_SxCR_CIRC;
    DMA2_Stream0->CR |= DMA_SxCR_MSIZE_1;
    DMA2_Stream0->CR |= DMA_SxCR_EN;
}