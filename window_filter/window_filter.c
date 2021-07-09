#include "window_filter.h"

// define constructor
void WindowFilter_Constructor(window_filter_t * const instance, const float init_value, const float window_size) {
    instance->window_size = window_size;

    WindowFilter_Reset(instance, init_value);
}

void WindowFilter_Reset(window_filter_t * const instance, const float init_value) {
    instance->value = init_value;
    instance->value_z = init_value;
}

// define window filter function
void WindowFilter_Step(window_filter_t * const instance, float * const input, const bool reset) {
    
    const float delta_in_consecutive_states = *input - instance->value;

    // if reset is true, then call reset function, else perform window filtering
    if (reset)
        WindowFilter_Reset(instance, *input);
    
    else {
        // if input is inside range, store input into window filter state variable
        if(
            ( delta_in_consecutive_states <= instance->window_size ) &&
            ( delta_in_consecutive_states >= -instance->window_size )
            )
            instance->value = *input;
        
        else
            instance->value = instance->value_z;
        
        // store input for next time step
        instance->value_z = *input;
    }
}