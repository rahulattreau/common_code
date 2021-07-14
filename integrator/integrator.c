#include "integrator.h"

void Integrator_Constructor(integrator_t * const instance, const float time_step) {
    
    // initialize the state variables
    UnitDelay_Constructor( &(instance->yk_1_) );
    instance->time_step_ = time_step;
    
}

void Integrator_Step(integrator_t * const instance, const float xk, const bool reset) {
    
    float input = 0.0;

    // execute unit delay step
    UnitDelay_Step( &(instance->yk_1_), xk, reset );

    // execute integrator function
    if (!reset)
        input = xk;
        
    instance->yk_ = input * instance->time_step_ + instance->yk_1_.yk_;
    
    // execute unit delay post step
    UnitDelay_PostStep( &(instance->yk_1_), instance->yk_ );
}