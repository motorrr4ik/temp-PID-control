#include "heating_cell.h"
#include "pid_regulator.h"

void switchPeltierMode(heating_cell_t *heating_cell)
{
}

void calculatePeltierPower(heating_cell_t *heating_cell)
{
    for (int8_t i = 0; i < HEATING_CELL_NUMBER; ++i)
    {
        heating_cell->pid_regulator.current_error = heating_cell->temperature.difference;
        heating_cell->peltier.power               = calculateDutyCycle(&(heating_cell->pid_regulator));
    }
}