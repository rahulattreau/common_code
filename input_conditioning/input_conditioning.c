#include "input_conditioning.h"

void InputConditioning_Constructor(
    input_conditioning_t * const instance, 
    const float window_size,
    const float time_step,
    const float tau
    ) {

    WindowFilter_Constructor( &(instance->window_filter_object), window_size);
    LowPassFilterO1_Constructor( &(instance->low_pass_filter_object), time_step, tau);

}

void InputConditioning_Init(input_conditioning_t * const instance, const float xk) {

    WindowFilter_Init( &(instance->window_filter_object), xk );
    LowPassFilterO1_Init( &(instance->low_pass_filter_object), xk );
    
}

void InputConditioning_Step(input_conditioning_t * const instance, const float xk, const bool reset) {
    
    WindowFilter_Step( &(instance->window_filter_object) , xk, reset );
    LowPassFilterO1_Step( &(instance->low_pass_filter_object), instance->window_filter_object.yk_, reset );

}