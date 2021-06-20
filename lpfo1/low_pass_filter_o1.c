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
    LowPassFilterO1_Reset(instance, xk);
}

void LowPassFilterO1_Reset(low_pass_filter_o1_t * const instance, const float xk) {
    // initialize the state variables
    instance->yk_1_ = xk;
    instance->yk_ = xk;
}

void LowPassFilterO1_Step(low_pass_filter_o1_t * const instance, const float xk, const bool reset) {
    // reset the lpf if there is a reset request, else calculate low pass filter value
    if(reset)
        LowPassFilterO1_Reset(instance, xk);    

    else {
        instance->yk_ = instance->yk_1_ + instance->alpha_ * ( xk - instance->yk_1_ ); // calculate lpf value
        instance->yk_1_ = instance->yk_; // record value for next time step
    }
}