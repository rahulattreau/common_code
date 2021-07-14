#include "input_conditioning.h"

// declare private functions

bool InputConditioning_WindowFilterReset(const bool window_filter_active, const bool reset);

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
    instance->window_filter_active_ = window_filter_active;

}

void InputConditioning_Init(input_conditioning_t * const instance, const float xk) {

    WindowFilter_Init( &(instance->window_filter_object_), xk );
    LowPassFilterO1_Init( &(instance->low_pass_filter_object_), xk );
    instance->yk_ = instance->low_pass_filter_object_.yk_;
    
}

void InputConditioning_Step(input_conditioning_t * const instance, const float xk, const bool reset) {

    const bool window_filter_reset = InputConditioning_WindowFilterReset(instance->window_filter_active_, reset);
    WindowFilter_Step( &(instance->window_filter_object_) , xk, window_filter_reset );
    LowPassFilterO1_Step( &(instance->low_pass_filter_object_), instance->window_filter_object_.yk_, reset );
    instance->yk_ = instance->low_pass_filter_object_.yk_;

}

bool InputConditioning_WindowFilterReset(const bool window_filter_active, const bool reset) {

    // truth table for window_filter_active and reset
    // window_filter_active	reset	window_filter_reset
    //            0	          0	           1
    //            0	          1	           1
    //            1	          0	           0
    //            1	          1	           1

    bool window_filter_reset;

    if (window_filter_active)
        window_filter_reset = reset;
    else
        window_filter_reset = true;
    
    return window_filter_reset;

}