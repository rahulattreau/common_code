#include "input_conditioning.h"

// declare private functions

bool InputConditioning_WindowFilterResetEval(const bool window_filter_active, const bool reset);

// declare public functions

void InputConditioning_Constructor(
    input_conditioning_t * const instance, 
    const float window_size,
    const float time_step,
    const float tau, 
    const bool window_filter_active
    ) {

    WindowFilter_Constructor( &(instance->window_filter_object_), window_size);
    LowPassFilterO1_Constructor( &(instance->low_pass_filter_object_), time_step, tau);
    instance->window_filter_enable_ = window_filter_active;
    
}

bool InputConditioning_WindowFilterResetEval(const bool window_filter_active, const bool reset) {

    /*
    description:
    1. the window filter is not active for all sensors.
    2. this function evaluates if what the reset value for the window filter needs to be, ...
    based on if the window filter is active and what the reset value is.

    truth table for window_filter_active and reset:
    window_filter_active	reset	window_filter_reset
               0	          0	           1
               0	          1	           1
               1	          0	           0
               1	          1	           1
    */

    bool window_filter_reset;

    if (window_filter_active)
        window_filter_reset = reset;
    else
        window_filter_reset = true;
    
    return window_filter_reset;

}

void InputConditioning_Step(input_conditioning_t * const instance, const float u) {

    bool reset = false; // assume data is valid, i.e. no error checking
    const bool window_filter_reset = InputConditioning_WindowFilterResetEval(instance->window_filter_enable_, reset);
    WindowFilter_Step( &(instance->window_filter_object_) , u, window_filter_reset );
    LowPassFilterO1_Step( &(instance->low_pass_filter_object_), instance->window_filter_object_.yk_, reset );
    instance->yk_ = instance->low_pass_filter_object_.yk_;

}