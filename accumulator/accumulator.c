#include "control_common_code/accumulator.h"

void AccumulatorInit(accumulator_t * const instance) {
    
    // initialize the state variables
    ResetManagerInit( &(instance->reset_manager) );
    UnitDelayInit( &(instance->output_k_1) );
    
}

void AccumulatorStep(accumulator_t * const instance, const float xk, const float init_val, const bool reset) {
    
    float input = 0.0;
    
    ResetManagerStep( &(instance->reset_manager), reset );
    UnitDelayStep( &(instance->output_k_1), init_val, instance->reset_manager.reset_state );

    // execute accumulator function
    if (! (instance->reset_manager.reset_state) )
        input = xk;
        
    instance->output = input + instance->output_k_1.output;
    
    // execute unit delay post step
    UnitDelayPostStep( &(instance->output_k_1), instance->output );

}