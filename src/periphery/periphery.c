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
    DMA1SetUp((uint32_t) & (stand->input_data), (uint32_t) & (stand->output_data));
    DMA2SetUp((uint32_t) & (stand->adc), (uint32_t) & (stand->spi_adc));
    ADC1SetUp();
    SPI1SetUp();
}

void EnablePeriphery()
{
}

void DisablePeriphery()
{
}