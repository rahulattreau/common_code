#include "accumulator.h"

void Accumulator_Constructor(accumulator_t * const instance) {
    
    // initialize the state variables
    UnitDelay_Constructor( &(instance->yk_1_) );
    
}

void Accumulator_Step(accumulator_t * const instance, const float xk, const float init_val, const bool reset) {
    
    float input = 0.0;
    bool reset_accumulator = !(instance->yk_1_.init_) || reset; // this ensures reset in first time step

    // execute unit delay step
    UnitDelay_Step( &(instance->yk_1_), init_val, reset );

    // execute accumulator function
    if (!reset_accumulator)
        input = xk;
        
    instance->yk_ = input + instance->yk_1_.yk_;
    
    // execute unit delay post step
    UnitDelay_PostStep( &(instance->yk_1_), instance->yk_ );

}