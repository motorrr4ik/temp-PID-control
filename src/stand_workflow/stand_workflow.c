#include "stand_workflow.h"
#include "stm32f4xx.h"

static void _initCells(stand_workflow_t *stand)
{
    // cells initiation - hardcode
    // to understand pins bindings check out ../pinout_info folder
    heating_cell_t first_cell   = {.cell_number   = 1,
                                   .peltier       = {.power         = 0,
                                                     .timer_channel = (uint32_t *)&(TIM1->CCR4),
                                                     .gpio_sw       = (uint32_t *)&(GPIOC->BSRR),
                                                     .cool_mode     = GPIO_BSRR_BS7,
                                                     .heat_mode     = GPIO_BSRR_BR7},
                                   .light         = {0},
                                   .temperature   = {0},
                                   .pid_regulator = {.pid_enable_border_temperature = DEFAULT_PID_ENABLE_BORDER, 0},
                                   .status        = standby};

    heating_cell_t second_cell  = {.cell_number   = 2,
                                   .peltier       = {.power         = 0,
                                                     .timer_channel = (uint32_t *)&(TIM1->CCR3),
                                                     .gpio_sw       = (uint32_t *)&(GPIOB->BSRR),
                                                     .cool_mode     = GPIO_BSRR_BS6,
                                                     .heat_mode     = GPIO_BSRR_BR6},
                                   .light         = {0},
                                   .temperature   = {0},
                                   .pid_regulator = {.pid_enable_border_temperature = DEFAULT_PID_ENABLE_BORDER, 0},
                                   .status        = standby};

    heating_cell_t third_cell   = {.cell_number   = 3,
                                   .peltier       = {.power         = 0,
                                                     .timer_channel = (uint32_t *)&(TIM1->CCR2),
                                                     .gpio_sw       = (uint32_t *)&(GPIOB->BSRR),
                                                     .cool_mode     = GPIO_BSRR_BS3,
                                                     .heat_mode     = GPIO_BSRR_BR3},
                                   .light         = {0},
                                   .temperature   = {0},
                                   .pid_regulator = {.pid_enable_border_temperature = DEFAULT_PID_ENABLE_BORDER, 0},
                                   .status        = standby};

    heating_cell_t third_cell   = {.cell_number   = 3,
                                   .peltier       = {.power         = 0,
                                                     .timer_channel = (uint32_t *)&(TIM1->CCR2),
                                                     .gpio_sw       = (uint32_t *)&(GPIOB->BSRR),
                                                     .cool_mode     = GPIO_BSRR_BS3,
                                                     .heat_mode     = GPIO_BSRR_BR3},
                                   .light         = {0},
                                   .temperature   = {0},
                                   .pid_regulator = {.pid_enable_border_temperature = DEFAULT_PID_ENABLE_BORDER, 0},
                                   .status        = standby};

    heating_cell_t fourth_cell  = {.cell_number   = 4,
                                   .peltier       = {.power         = 0,
                                                     .timer_channel = (uint32_t *)&(TIM1->CCR1),
                                                     .gpio_sw       = (uint32_t *)&(GPIOA->BSRR),
                                                     .cool_mode     = GPIO_BSRR_BS10,
                                                     .heat_mode     = GPIO_BSRR_BR10},
                                   .light         = {0},
                                   .temperature   = {0},
                                   .pid_regulator = {.pid_enable_border_temperature = DEFAULT_PID_ENABLE_BORDER, 0},
                                   .status        = standby};

    heating_cell_t fifth_cell   = {.cell_number   = 5,
                                   .peltier       = {.power         = 0,
                                                     .timer_channel = (uint32_t *)&(TIM3->CCR2),
                                                     .gpio_sw       = (uint32_t *)&(GPIOB->BSRR),
                                                     .cool_mode     = GPIO_BSRR_BS14,
                                                     .heat_mode     = GPIO_BSRR_BR14},
                                   .light         = {0},
                                   .temperature   = {0},
                                   .pid_regulator = {.pid_enable_border_temperature = DEFAULT_PID_ENABLE_BORDER, 0},
                                   .status        = standby};

    heating_cell_t sixth_cell   = {.cell_number   = 6,
                                   .peltier       = {.power         = 0,
                                                     .timer_channel = (uint32_t *)&(TIM3->CCR1),
                                                     .gpio_sw       = (uint32_t *)&(GPIOB->BSRR),
                                                     .cool_mode     = GPIO_BSRR_BS2,
                                                     .heat_mode     = GPIO_BSRR_BR2},
                                   .light         = {0},
                                   .temperature   = {0},
                                   .pid_regulator = {.pid_enable_border_temperature = DEFAULT_PID_ENABLE_BORDER, 0},
                                   .status        = standby};

    heating_cell_t seventh_cell = {.cell_number   = 7,
                                   .peltier       = {.power         = 0,
                                                     .timer_channel = (uint32_t *)&(TIM3->CCR3),
                                                     .gpio_sw       = (uint32_t *)&(GPIOB->BSRR),
                                                     .cool_mode     = GPIO_BSRR_BS12,
                                                     .heat_mode     = GPIO_BSRR_BR12},
                                   .light         = {0},
                                   .temperature   = {0},
                                   .pid_regulator = {.pid_enable_border_temperature = DEFAULT_PID_ENABLE_BORDER, 0},
                                   .status        = standby};

    heating_cell_t eigth_cell   = {.cell_number   = 8,
                                   .peltier       = {.power         = 0,
                                                     .timer_channel = (uint32_t *)&(TIM3->CCR4),
                                                     .gpio_sw       = (uint32_t *)&(GPIOA->BSRR),
                                                     .cool_mode     = GPIO_BSRR_BS12,
                                                     .heat_mode     = GPIO_BSRR_BR12},
                                   .light         = {0},
                                   .temperature   = {0},
                                   .pid_regulator = {.pid_enable_border_temperature = DEFAULT_PID_ENABLE_BORDER, 0},
                                   .status        = standby};
    stand->cells[0]             = first_cell;
    stand->cells[1]             = second_cell;
    stand->cells[2]             = third_cell;
    stand->cells[3]             = fourth_cell;
    stand->cells[4]             = fifth_cell;
    stand->cells[5]             = sixth_cell;
    stand->cells[6]             = seventh_cell;
    stand->cells[7]             = eigth_cell;
}

static void _handlePeriod(stand_workflow_t *stand)
{
}

static void _calculateTemperatureEquation(stand_workflow_t *stand)
{
    // TO-DO: calculation algo needed
}

static void _calculateTemperature(stand_workflow_t *stand)
{
    for (int8_t i = 0; i < HEATING_CELL_NUMBER; ++i)
    {
        _calculateTemperatureEquation(stand);
    }
}

void mainTask(stand_workflow_t *stand)
{
    if (!(stand->cells_initiated))
    {
        _initCells(stand);
        stand->cells_initiated = 1;
    }
    _calculateTemperature(stand);
}