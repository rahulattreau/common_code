#pragma once
#include "../accumulator/accumulator.h"

//define data type
typedef struct {
    accumulator_t accumulator;
    float time_step_;
} integrator_t;

void Integrator_Constructor(integrator_t * const instance, const float time_step);

void Integrator_Step(integrator_t * const instance, const float xk, const float init_val, const bool reset);