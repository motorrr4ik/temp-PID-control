#ifndef UTILS
#define UTILS
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
//Peripheral macros
#define SYSTEM_CORE_CLOCK 16000000
#define TIM3_PSC 160
#define TIM3_ARR 1000
#define TIM4_PSC 160
#define TIM4_ARR 1000

//Logical macros
#define REF_VOLTAGE 3.3
#define MAX_ADC_VALUE 4096
#define AIM_TEMP_UPPER 55
#define AIM_TEMP_LOWER 30
#define TEMP_DELTA 0.15
#define ACHIEVED_TEMP_KEEPER 385
#define ERROR_BORDER 5
#define K_P 35.0
#define K_I 50.0
#define K_E 0.01
#define dT  0.01

//structs and datatypes
typedef struct{
    int8_t value[3];
}message;
#endif