#include "window_filter.h"

// define initialization function
void WindowFilterInit(float * const state_variable, const float init_value) {
    *state_variable = init_value;
}

// define window filter function
void WindowFilterFunc(float * const state_variable, float_state_and_state_z * const input_value, const float window_size) {
    
    float difference_in_values = input_value->value - *state_variable;

    // if input is inside range, store input into window filter state variable
    if(
        ( difference_in_values <= window_size ) ||
        ( difference_in_values >= -window_size )
        )
        
        *state_variable = input_value->value;

}