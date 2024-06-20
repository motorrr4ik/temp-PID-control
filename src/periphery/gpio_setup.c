#include "periphery.h"

void GPIOASetup(void)
{
}

void GPIOBSetup(void)
{
    GPIOB->MODER |= GPIO_MODER_MODE5_1;
    GPIOB->AFR[0] |= GPIO_AFRL_AFRL5_1;
}

void GPIOCSetup(void)
{
    //-----------------------------GPIOC General IO----------------------------------
    GPIOC->MODER |= GPIO_MODER_MODE7_0;
    //-----------------------------GPIOC TIM3 setting up-----------------------------
    GPIOC->MODER |= GPIO_MODER_MODE6_1; // Set PC6 to alternate function mode
    GPIOC->MODER |= GPIO_MODER_MODE8_1; // Set PC8 to alternate function mode
    GPIOC->MODER |= GPIO_MODER_MODE9_1; // Set PC9 to alternate function mode
    GPIOC->AFR[0] |= GPIO_AFRL_AFRL6_1; // Enable alternate function register for PC6 pin, PWM3_1
    GPIOC->AFR[1] |= GPIO_AFRH_AFRH0_1; // Enable alternate function register for PC8 pin, PWM3_3
    GPIOC->AFR[1] |= GPIO_AFRH_AFRH1_1; // Enable alternate function register for PC9 pin, PWM3_4
    //-----------------------------GPIOC Analog setting up---------------------------
    GPIOC->MODER |= GPIO_MODER_MODE0_1 | GPIO_MODER_MODE0_0;
    GPIOC->MODER |= GPIO_MODER_MODE1_1 | GPIO_MODER_MODE1_0;
    GPIOC->MODER |= GPIO_MODER_MODE2_1 | GPIO_MODER_MODE2_0;
    GPIOC->MODER |= GPIO_MODER_MODE3_1 | GPIO_MODER_MODE3_0;
    GPIOC->MODER |= GPIO_MODER_MODE4_1 | GPIO_MODER_MODE4_0;
}