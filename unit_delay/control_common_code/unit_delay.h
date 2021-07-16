/*
description:
1. Implements a unit delay
2. Stores the input from the previous time step
3. Has a 'reset' input. If reset is true, the output is set to the input of the current time step.
4. When reset is false, the output is set to the input from the previous time step.
5. Gets initialized in the first time step. This happens by using the initialized bit.
*/

#ifndef UNIT_DELAY_H
#define UNIT_DELAY_H

#include <stdbool.h>
#include "../reset_manager"

// define data type
typedef struct {
    float output;
    float input_stored;
    bool initialized;
} unit_delay_t;

/* 
declare functions:
Constructor - use this when declaring variable of this class type
Step function
Post step function - calls Storestate
*/

void UnitDelayInit(unit_delay_t * const instance);

void UnitDelayStep(unit_delay_t * const instance, const float input, const bool reset);

void UnitDelayPostStep(unit_delay_t * const instance, const float input);

#endif // UNIT_DELAY_H