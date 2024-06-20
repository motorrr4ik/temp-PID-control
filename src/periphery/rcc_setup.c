#include "periphery.h"
void RCCSetUp(void)
{
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;   // ADC1 activation
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;  // GPIOA ports activation
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;  // GPIOB ports activation
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;  // GPIOC ports activation
    RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;   // TIM1 activation
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;   // TIM3 activation
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN; // USART2 activation
    RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;   // DMA1 activation
    RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;   // DMA2 activation
}