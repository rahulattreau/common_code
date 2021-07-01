/*
Description:
1. If absolute difference in value from current time step to previous time step of input 
is greater than window size, retain window state variable value.
2. Else, use new input value
*/

#pragma once

            // #include "../data_type_state_variable/float_state_and_state_z.h"

// define data type
typedef struct {
    float value;
    float value_z;
} window_filter_t;

// declare initialization function
void WindowFilter_Constructor(float * const state_variable, const float init_value);

// declare window filter function
void WindowFilter_Step(window_filter_t * const input_value, float * const instance, const float window_size);