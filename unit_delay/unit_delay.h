/*
description:
1. Implements a unit delay
2. Stores the input from the previous time step
3. Has a 'reset' input. If reset is true, the output is set to the input of the current time step.
4. When reset is false, the output is set to the input from the previous time step.
*/

#pragma once
#include <stdbool.h>

// define data type
typedef struct {
    float yk_;
    float xk_stored;
} unit_delay_t;

/* 
declare functions:
Constructor - use this when declaring variable of this class type
Initialization function - points to reset function.
    - Use this for assigning value for first time step.
    - This is kept separate from the constructor as when declaring a variable, data might not be available
Reset function - private function for resetting state internal state and output to input value
Post step function
    - points to StoreState
    - this runs at the end of a time step
*/

void UnitDelay_Constructor(unit_delay_t * const instance);

void (*UnitDelay_Init)(unit_delay_t * const, const float);

void UnitDelay_Step(unit_delay_t * const instance, const float xk, const bool reset);

void (*UnitDelay_PostStep)(unit_delay_t * const, const float);