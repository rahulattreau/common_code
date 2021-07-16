#pragma once
#include "../../unit_delay/control_common_code/unit_delay.h"
#include "../../reset_manager/reset_manager.h"

//define data type
typedef struct {
    float yk_;
    unit_delay_t yk_1_;
    reset_manager_t reset_manager_;
    
} accumulator_t;

void Accumulator_Constructor(accumulator_t * const instance);

void Accumulator_Step(accumulator_t * const instance, const float xk, const float init_val, const bool reset);