#include "control_common_code/window_filter.h"

// define constructor
void WindowFilter_Constructor(window_filter_t * const instance, const float window_size) {

    instance->window_size = window_size;
    UnitDelay_Constructor( &(instance->output_k_1) );

}

// define window filter function
void WindowFilter_Step(window_filter_t * const instance, const float input, const bool reset) {
    
    const float delta_in_consecutive_states = input - instance->output;

    // run unit delay
    UnitDelay_Step( &(instance->output_k_1), input, reset );

    // if input is inside range, set output to input, else set output to unit delayed value
    if( ( delta_in_consecutive_states <= instance->window_size ) &&
        ( delta_in_consecutive_states >= -instance->window_size ) )
        
        instance->output = input;
    
    else
        instance->output = instance->output_k_1.output;
    
    // run post step functions
    UnitDelay_PostStep( &(instance->output_k_1), input );
    
}