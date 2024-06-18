#include "pid_regulator.h"
#include "utils.h"

uint16_t calculateDutyCycle(pid_regulator_t *pid_regulator)
{

    if (pid_regulator->current_error > pid_regulator->pid_enable_border_temperature)
    {
        pid_regulator->duty_cycle = TIM3_ARR;
    }
    else
    {
        // Check if intergral error between min and max duty cycle value
        if (((pid_regulator->k_i * pid_regulator->integral_error <= TIM3_ARR) && pid_regulator->current_error >= 0) ||
            ((pid_regulator->k_i * pid_regulator->integral_error >= 0) && pid_regulator->current_error < 0))
        {
            pid_regulator->integral_error += pid_regulator->integral_error / 20;
        }
        pid_regulator->diff_error = (pid_regulator->current_error - pid_regulator->previous_error) / dT;

        pid_regulator->duty_cycle = pid_regulator->k_p * pid_regulator->current_error +
                                    pid_regulator->k_i * pid_regulator->integral_error +
                                    pid_regulator->k_d * pid_regulator->diff_error;

        pid_regulator->previous_error = pid_regulator->current_error;
    }
    return pid_regulator->duty_cycle;
}