/*
Description:
1. If absolute difference in value from current time step to previous time step of input 
is greater than window size, retain window state variable value.
2. Else, use new input value
*/

#pragma once

#include <stdbool.h>
#include "../unit_delay/unit_delay.h"

// define data type
typedef struct {
    float yk_; // output variable
    unit_delay_t yk_1_; // output variable from previous time step
    float window_size_; // window size
} window_filter_t;

// declare constructor
void WindowFilter_Constructor(window_filter_t * const instance, const float window_size);

// declare initialization function
void WindowFilter_Init(window_filter_t * const instance, const float xk);

// declare window filter function
void WindowFilter_Step(window_filter_t * const instance, const float xk, const bool reset);