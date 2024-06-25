#include "periphery.h"

void InitPeriphery(stand_workflow_t *stand)
{
    RCCSetUp();
    GPIOASetup();
    GPIOBSetup();
    GPIOCSetup();
    TIM1SetUp();
    TIM3SetUp();
    USART2SetUp();
    DMA1SetUp(&(stand->input_data), &(stand->output_data));
    DMA2SetUp(&(stand->adc), &(stand->spi_adc));
    ADC1SetUp();
    SPI1SetUp();
}

void EnablePeriphery()
{
}

void DisablePeriphery()
{
}