#pragma once
#include "../unit_delay/unit_delay.h"

//define data type
typedef struct {
    float yk_;
    float time_step_;
    unit_delay_t yk_1_;
} integrator_t;

void Integrator_Constructor(integrator_t * const instance, const float time_step);

void Integrator_Step(integrator_t * const instance, const float xk, const bool reset);