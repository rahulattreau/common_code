#include "control_common_code/low_pass_filter_order_1.h"

void LowPassFilterOrder1Init(
    low_pass_filter_o1_t * const instance, 
    const float time_step, 
    const float tau
    ) {
    
    // set alpha
    instance->alpha = time_step / ( time_step + tau );
    // initialize the state variables
    UnitDelayInit( &(instance->output_k_1) );
    
}

void LowPassFilterOrder1Step(low_pass_filter_o1_t * const instance, const float input, const bool reset) {

    // execute unit delay step
    UnitDelayStep( &(instance->output_k_1), input, reset );

    // execute lpf function
    instance->output = instance->alpha * input + (1 - instance->alpha) * instance->output_k_1.output;
    
    // execute unit delay post step
    UnitDelayPostStep( &(instance->output_k_1), instance->output );

}