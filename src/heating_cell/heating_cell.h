#ifndef HEATING_CELL
#define HEATING_CELL

#include "pid_regulator.h"
#include <stdint.h>
#define HEATING_CELL_NUMBER 8

typedef enum
{
    cooling = 0,
    heating = 1
} cell_status;

typedef struct
{
    int8_t current_temperature;
    int8_t aim_temperature;
    int8_t difference;
} temperature_state_t;

typedef struct
{
    uint32_t power;
    uint32_t switch_pin;
} peltier_state_t;

typedef struct
{
    uint32_t light_value;
    // uint32_t light_pin;
} light_state_t;

typedef struct
{
    uint8_t cell_number;
    peltier_state_t peltier;
    light_state_t light;
    temperature_state_t temperature;
    pid_regulator_t pid_regulator;
    cell_status status;
} heating_cell_t;

void switchPeltierMode(heating_cell_t *heating_cell);
void calculatePeltierPower(heating_cell_t *heating_cell);

#endif // HEATING_CELL