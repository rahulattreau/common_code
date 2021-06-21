#include "window_filter.h"

// define initialization function
void WindowFilterInit(float * const state_variable, const float init_value) {
    *state_variable = init_value;
}

// define window filter function
void WindowFilterFunc(float * const state_variable, float_state_and_state_z * const input_value, const float window_size) {
    
    if(
        !(
        ( (input_value->value - input_value->value_z) > window_size ) ||
        ( (input_value->value - input_value->value_z) < -window_size )
        )
        )
        
        *state_variable = input_value->value;

}