#include "periphery.h"
#include "utils.h"

void TIM1SetUp(void)
{
    TIM1->CR1 |= TIM_CR1_CEN;
    TIM1->PSC = TIM3_PSC - 1;       // Set TIM3 prescalser: check out utils.h file for values
    TIM1->ARR = TIM3_ARR - 1;       // Set TIM3 arr: check out utils.h file for values
    TIM1->CNT = 0;                  // Set TIM3 counter
    TIM1->CCMR1 |= TIM_CCMR1_OC1M_1 // Enable PWM generation on TIM3 Channel1
                   | TIM_CCMR1_OC1M_2;

    TIM1->CCMR2 |= TIM_CCMR2_OC3M_1 // Enable PWM generation on TIM3 Channel1
                   | TIM_CCMR2_OC3M_2;

    TIM1->CCER |= TIM_CCER_CC1E; // Enable capture/compare register
    TIM1->CCER |= TIM_CCER_CC2E; // Enable capture/compare register
    TIM1->CCER |= TIM_CCER_CC3E; // Enable capture/compare register
    TIM1->CCER |= TIM_CCER_CC4E; // Enable capture/compare register
    TIM1->CCR1 = 0;
    TIM1->CCR2 = 0;
    TIM1->CCR3 = 0;
    TIM1->CCR4 = 0;
}

void TIM3SetUp(void)
{
    TIM3->CR1 |= TIM_CR1_CEN;       // Enable TIM3
    TIM3->PSC = TIM3_PSC - 1;       // Set TIM3 prescalser: check out utils.h file for values
    TIM3->ARR = TIM3_ARR - 1;       // Set TIM3 arr: check out utils.h file for values
    TIM3->CNT = 0;                  // Set TIM3 counter
    TIM3->CCMR1 |= TIM_CCMR1_OC1M_1 // Enable PWM generation on TIM3 Channel1
                   | TIM_CCMR1_OC1M_2;

    TIM3->CCMR2 |= TIM_CCMR2_OC3M_1 // Enable PWM generation on TIM3 Channel1
                   | TIM_CCMR2_OC3M_2;

    TIM3->CCER |= TIM_CCER_CC1E; // Enable capture/compare register
    TIM3->CCER |= TIM_CCER_CC2E; // Enable capture/compare register
    TIM3->CCER |= TIM_CCER_CC3E; // Enable capture/compare register
    TIM3->CCER |= TIM_CCER_CC4E; // Enable capture/compare register
    TIM3->CCR1 = 0;
    TIM3->CCR2 = 0;
    TIM3->CCR3 = 0;
    TIM3->CCR4 = 0;
}