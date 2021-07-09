/*
description:
1. Implements a unit delay
2. Stores the input from the previous time step
3. Has a 'reset' input. If reset is true, the output is set to the input of the current time step.
4. When reset is false, the output is set to the input from the previous time step.
*/

#pragma once
#include <stdbool.h>

// define public variables
typedef struct {
    float yk_;
    float xk_stored;
} unit_delay_t;

void UnitDelay_Constructor(unit_delay_t * const instance, const float init_value);

void UnitDelay_StoreState(unit_delay_t * const instance, const float xk);

void (*UnitDelay_PostStep)(unit_delay_t * const, const float);

void UnitDelay_Step(unit_delay_t * const instance, const float xk, const bool reset);