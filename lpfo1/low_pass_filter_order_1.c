#include "control_common_code/low_pass_filter_order_1.h"

void LowPassFilterO1_Constructor(
    low_pass_filter_o1_t * const instance, 
    const float time_step, 
    const float tau
    ) {
    
    // set alpha
    instance->alpha = time_step / ( time_step + tau );
    // initialize the state variables
    UnitDelay_Constructor( &(instance->output_k_1) );
    
}

void LowPassFilterO1_Step(low_pass_filter_o1_t * const instance, const float input, const bool reset) {

    // execute unit delay step
    UnitDelay_Step( &(instance->output_k_1), input, reset );

    // execute lpf function
    instance->output = instance->alpha * input + (1 - instance->alpha) * instance->output_k_1.output;
    
    // execute unit delay post step
    UnitDelay_PostStep( &(instance->output_k_1), instance->output );

}