#include "control_common_code/low_pass_filter_o1.h"

void LowPassFilterO1_Constructor(
    low_pass_filter_o1_t * const instance, 
    const float time_step, 
    const float tau
    ) {
    
    // set alpha_
    instance->alpha_ = time_step / ( time_step + tau );
    // initialize the state variables
    UnitDelay_Constructor( &(instance->yk_1_) );
    
}

void LowPassFilterO1_Step(low_pass_filter_o1_t * const instance, const float u, const bool reset) {

    // execute unit delay step
    UnitDelay_Step( &(instance->yk_1_), u, reset );

    // execute lpf function
    instance->yk_ = instance->alpha_ * u + (1 - instance->alpha_) * instance->yk_1_.yk_;
    
    // execute unit delay post step
    UnitDelay_PostStep( &(instance->yk_1_), instance->yk_ );

}