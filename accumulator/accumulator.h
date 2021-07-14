#pragma once
#include "../unit_delay/unit_delay.h"

//define data type
typedef struct {
    float yk_;
    unit_delay_t yk_1_;
} accumulator_t;

void Accumulator_Constructor(accumulator_t * const instance, const float time_step);

void Accumulator_Step(accumulator_t * const instance, const float xk, const float init_val, const bool reset);