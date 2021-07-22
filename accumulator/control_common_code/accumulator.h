#pragma once
#include "../../unit_delay/control_common_code/unit_delay.h"
#include "../../reset_manager/reset_manager.h"

//define data type
typedef struct {
    float output;
    unit_delay_t output_k_1;
    reset_manager_t reset_manager;
    
} accumulator_t;

void AccumulatorInit(accumulator_t * const instance);

void AccumulatorStep(accumulator_t * const instance, const float xk, const float init_val, const bool reset);