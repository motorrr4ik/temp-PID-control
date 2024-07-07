#include "periphery.h"

void initPeriphery(stand_workflow_t *stand)
{
    RCCSetUp();
    GPIOASetup();
    GPIOBSetup();
    GPIOCSetup();
    TIM1SetUp();
    TIM3SetUp();
    TIM4SetUp();
    USART2SetUp();
    DMA1SetUp((uint32_t) & (stand->input_data), (uint32_t) & (stand->output_data));
    DMA2SetUp((uint32_t) & (stand->adc), (uint32_t) & (stand->spi_adc));
    ADC1SetUp();
    SPI1SetUp();
}

void enablePeriphery()
{
    RCCSetUp();
}

void disablePeriphery()
{
    RCC->APB2ENR &= ~(RCC_APB2ENR_ADC1EN);  // ADC1 activation
    RCC->AHB1ENR &= ~(RCC_AHB1ENR_GPIOAEN); // GPIOA ports activation
    RCC->AHB1ENR &= ~(RCC_AHB1ENR_GPIOBEN); // GPIOB ports activation
    RCC->AHB1ENR &= ~(RCC_AHB1ENR_GPIOCEN); // GPIOC ports activation
    RCC->APB2ENR &= ~(RCC_APB2ENR_TIM1EN);  // TIM1 activation
    RCC->APB1ENR &= ~(RCC_APB1ENR_TIM3EN);  // TIM3 activation
    // RCC->APB1ENR |= RCC_APB1ENR_USART2EN; // USART2 activation
    RCC->AHB1ENR &= ~(RCC_AHB1ENR_DMA1EN); // DMA1 activation
    RCC->AHB1ENR &= ~(RCC_AHB1ENR_DMA2EN); // DMA2 activation
}