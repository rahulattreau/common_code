#include "low_pass_filter_o1.h"

void LowPassFilterO1_Constructor(
    low_pass_filter_o1_t * const instance, 
    const float xk, 
    const float time_step, 
    const float tau
    ) {
    
    // set alpha_
    instance->alpha_ = time_step / ( time_step + tau );
    // initialize the state variables
    UnitDelay_Constructor( &(instance->yk_1_), 0.0 );
    
}

void LowPassFilterO1_Step(low_pass_filter_o1_t * const instance, const float xk, const bool reset) {

    // execute unit delay step
    UnitDelay_Step( &(instance->yk_1_), xk, reset );

    // execute lpf function
    instance->yk_ = instance->alpha_ * xk + (1 - instance->alpha_) * instance->yk_1_.yk_;
    
    // execute unit delay post step
    UnitDelay_PostStep( &(instance->yk_1_), instance->yk_ );

}