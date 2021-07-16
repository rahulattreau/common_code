/*
description:
1. Implements a unit delay
2. Stores the input from the previous time step
3. Has a 'reset' input. If reset is true, the output is set to the input of the current time step.
4. When reset is false, the output is set to the input from the previous time step.
5. Gets initialized in the first time step. This happens by using the init_ bit.
*/

#ifndef UNIT_DELAY_H
#define UNIT_DELAY_H

#include <stdbool.h>
#include "../../reset_manager/reset_manager.h"

// define data type
typedef struct {
    float yk_;
    float u_stored;
    reset_manager_t reset_manager;
} unit_delay_t;

/* 
declare functions:
Constructor - use this when declaring variable of this class type
Step function
Post step function - calls Storestate
*/

void UnitDelay_Constructor(unit_delay_t * const instance);

void UnitDelay_Step(unit_delay_t * const instance, const float u, const bool reset);

void UnitDelay_PostStep(unit_delay_t * const instance, const float u);

#endif // UNIT_DELAY_H