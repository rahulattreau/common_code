#include "control_common_code/integrator.h"

void IntegratorInit(integrator_t * const instance, const float time_step) {
    
    AccumulatorInit( &(instance->accumulator) );
    
    // initialize the state variables
    instance->time_step = time_step;
    
}

void IntegratorStep(integrator_t * const instance, const float xk, const float init_val, const bool reset) {
    
    const float input = xk * instance->time_step;
    AccumulatorStep( &(instance->accumulator), input, init_val, reset);
    instance->output = instance->accumulator.output;

}