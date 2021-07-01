/*
Description:
1. If absolute difference in value from current time step to previous time step of input 
is greater than window size, retain window state variable value.
2. Else, use new input value
*/

#pragma once

#include "../data_type_state_variable/float_state_and_state_z.h"

// declare initialization function
void WindowFilterInit(float * const state_variable, const float init_value);

// declare window filter function
void WindowFilterFunc(float * const instance, float_state_and_state_z * const input_value, const float window_size);