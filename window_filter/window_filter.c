#include "window_filter.h"

// define initialization function
void WindowFilterInit(window_filter_t * const state_variable, const float init_value) {
    *state_variable = init_value;
}

// define window filter function
void WindowFilterFunc(window_filter_t * const state_variable, float_state_and_state_z * const input_value, const float window_size) {
    
    const float delta_in_consecutive_states = input_value->value - input_value->value_z;

    // if input is inside range, store input into window filter state variable
    if(
        ( delta_in_consecutive_states <= window_size ) ||
        ( delta_in_consecutive_states >= -window_size )
        )
        
        *state_variable = input_value->value;

}