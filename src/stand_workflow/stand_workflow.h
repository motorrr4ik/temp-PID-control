#ifndef STAND_WORKFLOW_H
#define STAND_WORKFLOW_H
#include "heating_cell.h"
#include "stdint.h"
#include "utils.h"

typedef enum
{
    stand_disable = 0,
    stand_enable,
    stand_set_params
} operation_command_e;

typedef struct
{
    uint32_t raw_spi_adc_data[HEATING_CELL_NUMBER];
} spi_adc_data_t;

typedef struct
{
    uint32_t raw_adc_data[HEATING_CELL_NUMBER];
} adc_data_t;

typedef struct
{
    operation_command_e command;
    int8_t data_buff[DATA_BUFF_SIZE];
} input_data_package_t;

typedef struct
{
    int8_t data_buff[DATA_BUFF_SIZE];
} output_data_package_t;

typedef struct
{
    int8_t if_enabled;
    int8_t cells_initiated;
    float temperature_equation_coeffs[TEMP_CALC_COEFFS];
    spi_adc_data_t spi_adc;
    adc_data_t adc;
    input_data_package_t input_data;
    output_data_package_t output_data;
    heating_cell_t cells[HEATING_CELL_NUMBER];
} stand_workflow_t;

void MainTask(stand_workflow_t *stand);
static void _InitCells(stand_workflow_t *stand);
static void _CalculateTemperature(stand_workflow_t *stand);
static void _HandlePeriod(stand_workflow_t *stand);
#endif