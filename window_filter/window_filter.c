#include "control_common_code/window_filter.h"

// define constructor
void WindowFilterInit(window_filter_t * const instance, const float window_size) {

    instance->window_size = window_size;
    UnitDelayInit( &(instance->output_k_1) );

}

// define window filter function
void WindowFilterStep(window_filter_t * const instance, const float input, const bool reset) {
    
    const float delta_in_consecutive_states = input - instance->output;

    // run unit delay
    UnitDelayStep( &(instance->output_k_1), input, reset );

    // if input is inside range, set output to input, else set output to unit delayed value
    if( ( delta_in_consecutive_states <= instance->window_size ) &&
        ( delta_in_consecutive_states >= -instance->window_size ) )
        
        instance->output = input;
    
    else
        instance->output = instance->output_k_1.output;
    
    // run post step functions
    UnitDelayPostStep( &(instance->output_k_1), input );
    
}