#include "periphery.h"
#include "pid_regulator.h"
#include "stand_workflow.h"
#include "stm32f4xx.h"

static stand_workflow_t stand = {.if_enabled                  = 0,
                                 .cells_initiated             = 0,
                                 .temperature_equation_coeffs = {9.418, -40.978, 95.315, -28.127},
                                 .spi_adc                     = 0,
                                 .adc                         = 0,
                                 .input_data                  = 0,
                                 .output_data                 = 0,
                                 .cells                       = 0};

int main(void)
{
    initPeriphery(&stand);
    while (1)
    {
        mainTask(&stand);
    }
    return 0;
}