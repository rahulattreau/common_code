#include "control_common_code/integrator.h"

void Integrator_Constructor(integrator_t * const instance, const float time_step) {
    
    Accumulator_Constructor( &(instance->accumulator) );
    
    // initialize the state variables
    instance->time_step_ = time_step;
    
}

void Integrator_Step(integrator_t * const instance, const float xk, const float init_val, const bool reset) {
    
    const float input = xk * instance->time_step_;
    Accumulator_Step( &(instance->accumulator), input, init_val, reset);
    instance->yk_ = instance->accumulator.yk_;

}