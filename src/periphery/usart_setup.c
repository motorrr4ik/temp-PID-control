#include "periphery.h"
#include "utils.h"

void USART2SetUp(void)
{
    USART2->CR1 |= USART_CR1_UE;                // USART enable
    USART2->CR1 |= USART_CR1_RE;                // USART recieve enable
    USART2->CR1 |= USART_CR1_TE;                // USART transmit enable
    USART2->CR1 |= USART_CR1_IDLEIE;            // USART IDLE interrupt enable
    USART2->CR3 |= USART_CR3_DMAR;              // USART DMA reading enable
    USART2->CR3 |= USART_CR3_DMAT;              // USART DMA transmitting enable
    USART2->BRR = SYSTEM_CORE_CLOCK / BAUDRATE; // Set USART speed
    NVIC_EnableIRQ(USART2_IRQn);
}