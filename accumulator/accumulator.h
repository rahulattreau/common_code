#pragma once
#include "../unit_delay/unit_delay.h"

//define data type
typedef struct {
    float yk_;
    unit_delay_t yk_1_;
} integrator_t;

void Accumulator_Constructor(integrator_t * const instance, const float time_step);

void Accumulator_Init(integrator_t * const instance, float xk);

void Accumulator_Step(integrator_t * const instance, const float xk, const bool reset);