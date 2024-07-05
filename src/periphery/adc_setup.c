#include "periphery.h"

void ADC1SetUp(void)
{
    // TO-DO check if pin sequence is correct
    ADC1->CR1 |= ADC_CR1_SCAN;  // Enable scan mode
    ADC1->SQR1 |= ADC_SQR1_L_3; // Select 8 channels to read from
    // ADC1->SQR3|= ADC_SQR3_SQ1_0;
    ADC1->SQR3 |= ADC_SQR3_SQ2_3 | ADC_SQR3_SQ2_0;
    ADC1->SQR3 |= ADC_SQR3_SQ3_3 | ADC_SQR3_SQ3_1;
    ADC1->SQR3 |= ADC_SQR3_SQ4_3 | ADC_SQR3_SQ4_1 | ADC_SQR3_SQ4_0;
    ADC1->SQR3 |= ADC_SQR3_SQ5_3 | ADC_SQR3_SQ5_2;
    ADC1->SQR3 |= ADC_SQR3_SQ6_3 | ADC_SQR3_SQ6_2 | ADC_SQR3_SQ6_0;
    ADC1->SQR2 |= ADC_SQR2_SQ7_4 | ADC_SQR2_SQ7_2;
    ADC1->SQR2 |= ADC_SQR2_SQ8_0;
    ADC1->CR2 |= ADC_CR2_DMA;     // Enable DMA
    ADC1->CR2 |= ADC_CR2_ADON;    // Enable A/D converter
    ADC1->CR2 |= ADC_CR2_CONT;    // Enable continious conversion
    ADC1->CR2 |= ADC_CR2_SWSTART; // Start conversion of regular channels
}