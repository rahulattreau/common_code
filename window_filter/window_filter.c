#include "window_filter.h"

// define initialization function
void WindowFilterInit(float * const state_variable, const float init_value) {
    *state_variable = init_value;
}

// define window filter function
void WindowFilterFunc(float * const state_variable, window_filter_t * const input_value, const float window_size) {
    
    const float delta_in_consecutive_states = input_value->value - input_value->value_z;

    // if input is inside range, store input into window filter state variable
    if(
        ( delta_in_consecutive_states <= window_size ) &&
        ( delta_in_consecutive_states >= -window_size )
        )
        
        *state_variable = input_value->value;

}