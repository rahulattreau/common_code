/*
Description:
1. If absolute difference in value from current time step to previous time step of input 
is greater than window size, retain window state variable value.
2. Else, use new input value
*/

#ifndef WINDOW_FILTER_H
#define WINDOW_FILTER_H

#include <stdbool.h>
#include "unit_delay.h"

// define data type
typedef struct {
    float yk_; // output variable
    unit_delay_t yk_1_; // output variable from previous time step
    float window_size_; // window size
} window_filter_t;

/*
declare the following functions:
Constructor - use when defining class variable
Step - this is the step function that runs every time step
*/

void WindowFilter_Constructor(window_filter_t * const instance, const float window_size);

void WindowFilter_Step(window_filter_t * const instance, const float u, const bool reset);

#endif // WINDOW_FILTER_H