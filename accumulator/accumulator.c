#include "control_common_code/accumulator.h"

void Accumulator_Constructor(accumulator_t * const instance) {
    
    // initialize the state variables
    ResetManager_Constructor( &(instance->reset_manager_) );
    UnitDelay_Constructor( &(instance->yk_1_) );
    
}

void Accumulator_Step(accumulator_t * const instance, const float xk, const float init_val, const bool reset) {
    
    float input = 0.0;
    
    ResetManager_Step( &(instance->reset_manager_), reset );
    UnitDelay_Step( &(instance->yk_1_), init_val, instance->reset_manager_.reset_state_ );

    // execute accumulator function
    if (! (instance->reset_manager_.reset_state_) )
        input = xk;
        
    instance->yk_ = input + instance->yk_1_.yk_;
    
    // execute unit delay post step
    UnitDelay_PostStep( &(instance->yk_1_), instance->yk_ );

}