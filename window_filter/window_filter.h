/*
Description:
1. If absolute difference in value from current time step to previous time step of input 
is greater than window size, retain window state variable value.
2. Else, use new input value
*/

#include "../data_type_state_variable/float_state_and_state_z.h"

// define window filter data type
typedef float window_filter_t;

// declare initialization function
void WindowFilterInit(window_filter_t * const state_variable, const float init_value);

// declare window filter function
void WindowFilterFunc(window_filter_t * const instance, float_state_and_state_z * const input_value, const float window_size);