#pragma once

#include "../../accumulator/control_common_code/accumulator.h"

//define data type
typedef struct {
    float output; // output
    accumulator_t accumulator;
    float time_step;
} integrator_t;

void IntegratorInit(integrator_t * const instance, const float time_step);

void IntegratorStep(integrator_t * const instance, const float xk, const float init_val, const bool reset);