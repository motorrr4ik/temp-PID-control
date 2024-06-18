#ifndef PID_REGULATOR
#define PID_REGULATOR

#include <stdint.h>

typedef struct
{
    int8_t pid_enable_border_temperature;
    uint16_t k_p;
    uint16_t k_i;
    uint16_t k_d;
    float current_error;
    float previous_error;
    float integral_error;
    float diff_error;
    uint16_t duty_cycle;
} pid_regulator_t;

uint16_t calculateDutyCycle(pid_regulator_t *pid_regulator);
#endif