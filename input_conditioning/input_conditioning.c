#include "input_conditioning.h"

void InputConditioning_Constructor(
    input_conditioning_t * const instance, 
    const float init_value,
    const float window_size,
    const float time_step,
    const float tau
    ) {

    instance->window_size = window_size;
    instance->time_step = time_step;
    instance->tau = tau;

    WindowFilter_Constructor( &(instance->window_filter_object), init_value, instance->window_size);
    LowPassFilterO1_Constructor( &(instance->low_pass_filter_object), instance->window_filter_object.yk_, instance->time_step, instance->tau);

}

void InputConditioning_Reset(input_conditioning_t * const instance, const bool reset) {

    WindowFilter_Reset( &(instance->window_filter_object), reset );
    LowPassFilterO1_Reset( &(instance->low_pass_filter_object), reset );
    
}


void InputConditioning_Step(input_conditioning_t * const instance, const float xk, const bool reset) {
    
    WindowFilter_Step( &(instance->window_filter_object) , xk, reset );
    LowPassFilterO1_Step( &(instance->low_pass_filter_object), instance->window_filter_object.yk_, reset );

}