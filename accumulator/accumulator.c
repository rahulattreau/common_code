#include "accumulator.h"

void Accumulator_Constructor(integrator_t * const instance, const float time_step) {
    
    // initialize the state variables
    UnitDelay_Constructor( &(instance->yk_1_) );
    
}

void Accumulator_Init(integrator_t * const instance, const float init_val) {
    UnitDelay_Init( &(instance->yk_1_), init_val );
    instance->yk_ = instance->yk_1_.yk_;
}

void Accumulator_Step(integrator_t * const instance, const float xk, const float init_val, const bool reset) {
    
    float input = 0.0;

    // execute unit delay step
    UnitDelay_Step( &(instance->yk_1_), init_val, reset );

    // execute integrator function
    if (!reset)
        input = xk;
        
    instance->yk_ = input + instance->yk_1_.yk_;
    
    // execute unit delay post step
    UnitDelay_PostStep( &(instance->yk_1_), instance->yk_ );
}