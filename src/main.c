#include "periphery.h"
#include "pid_regulator.h"
#include "stand_workflow.h"
#include "stm32f4xx.h"

static stand_workflow_t stand = {
#ifdef DEBUG
    .if_enabled = 1,
#else
    .if_enabled = 0,
#endif
    .cells_initiated             = 0,
    .cycle_temperatures          = 0,
    .temperature_equation_coeffs = {9.418, -40.978, 95.315, -28.127},
    .spi_adc                     = 0,
    .adc                         = 0,
    .input_data                  = 0,
    .output_data                 = 0,
    .cells                       = 0};

int main(void)
{
    SysTick_Config(SYSTEM_CORE_CLOCK);
    initPeriphery(&stand);
    while (1)
    {
        mainTask(&stand);
    }
    return 0;
}

void TIM4_IRQHandler(void)
{
    TIM4->SR &= ~TIM_SR_UIF; // Reset interrupt flag
    calcaluteCellsPowerControl(&stand);
}

void USART2_IRQHandler(void)
{
    if (USART2->SR & USART_SR_IDLE)
    {
        (void)USART2->DR;
        switch (stand.input_data.command)
        {
        case stand_disable:
            disablePeriphery();
            break;
        case stand_enable:
            enablePeriphery();
            break;
        case stand_set_params:
            disablePeriphery();
            setStandParameters(&stand);
            enablePeriphery();
            break;
        default:
            break;
        }
    }
}

void SysTick_Handler(void)
{
    for (int8_t i = 0; i < HEATING_CELL_NUMBER; ++i)
    {
        if (stand.cells[i].status == freezed)
        {
            stand.cells[i].cycle_counter++;
        }
    }
}