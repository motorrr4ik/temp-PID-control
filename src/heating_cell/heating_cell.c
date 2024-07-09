#include "heating_cell.h"
#include "pid_regulator.h"

static void _switchPeltierMode(heating_cell_t *heating_cell);

static void _switchPeltierMode(heating_cell_t *heating_cell)
{
    if (heating_cell->temperature.difference > 0)
    {
        heating_cell->status = heating;
        *(heating_cell->peltier.gpio_sw) &= ~heating_cell->peltier.cool_mode;
        *(heating_cell->peltier.gpio_sw) |= heating_cell->peltier.heat_mode;
    }
    else
    {
        heating_cell->status = cooling;
        *(heating_cell->peltier.gpio_sw) &= ~heating_cell->peltier.heat_mode;
        *(heating_cell->peltier.gpio_sw) |= heating_cell->peltier.cool_mode;
    }
}

void calculatePeltierPower(heating_cell_t *heating_cell)
{
    if (heating_cell->temperature.difference <= PID_SW_TEMP_DELTA)
    {
        heating_cell->status = freezed;
        return;
    }
    _switchPeltierMode(heating_cell);
    if (heating_cell->status == heating)
    {
        heating_cell->pid_regulator.current_error = heating_cell->temperature.difference;
        heating_cell->peltier.power               = calculateDutyCycle(&(heating_cell->pid_regulator));
    }
    else
    {
        heating_cell->pid_regulator.current_error = 0 - heating_cell->temperature.difference;
        heating_cell->peltier.power               = calculateDutyCycle(&(heating_cell->pid_regulator));
    }
    *(heating_cell->peltier.timer_channel) |= heating_cell->peltier.power;
}

void disablePeltier(heating_cell_t *heating_cell)
{
    *(heating_cell->peltier.gpio_sw) = 0;
}

uint16_t updateCycle(heating_cell_t *heating_cell)
{
    if (heating_cell->cycle_counter >= CYCLE_DURATION)
    {
        heating_cell->cycle_counter = 0;
        heating_cell->status        = standby;
        return 0;
    }
    return 1;
}