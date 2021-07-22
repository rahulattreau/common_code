#include "control_common_code/input_conditioning.h"

// declare private functions

bool InputConditioningWindowFilterResetEval(const bool window_filter_active, const bool reset);

// declare public functions

void InputConditioningInit(
    input_conditioning_t * const instance, 
    const float window_size,
    const float time_step,
    const float tau, 
    const bool window_filter_active
    ) {

    WindowFilterInit( &(instance->window_filter_object), window_size);
    LowPassFilterOrder1Init( &(instance->low_pass_filter_object), time_step, tau);
    instance->window_filter_enable = window_filter_active;
    
}

bool InputConditioningWindowFilterResetEval(const bool window_filter_active, const bool reset) {

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

void InputConditioningStep(input_conditioning_t * const instance, const float u) {

    bool reset = false; // assume data is valid, i.e. no error checking
    const bool window_filter_reset = InputConditioningWindowFilterResetEval(instance->window_filter_enable, reset);
    WindowFilterStep( &(instance->window_filter_object) , u, window_filter_reset );
    LowPassFilterOrder1Step( &(instance->low_pass_filter_object), instance->window_filter_object.output, reset );
    instance->output = instance->low_pass_filter_object.output;

}