#ifndef UTILS_H
#define UTILS_H
// Logical macros
#define REF_VOLTAGE          3.3
#define MAX_ADC_VALUE        4096
#define PID_SW_TEMP_DELTA    0.15
#define dT                   0.01
#define CYCLES_NUMBER        3
#define HEATING_CELL_NUMBER  8
#define DATA_BUFF_SIZE       200
#define TEMP_CALC_COEFFS     4
// Periphery macros
#define SYSTEM_CORE_CLOCK    16000000
#define TIM3_PSC             160
#define TIM3_ARR             1000
#define TIM1_PSC             160
#define TIM1_ARR             1000
#define BAUDRATE             9600
#define MAX_DUTY_CYCLE_VALUE 1000
#endif