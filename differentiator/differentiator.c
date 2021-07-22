#include "differentiator.h"

void DifferentiatorInit(differentiator_t * const instance, const float time_step) {

    UnitDelayInit( &(instance->output_k_1) );
    instance->time_step = time_step;

}

void DifferentiatorStep(differentiator_t * const instance, const float u, const bool reset) {

    UnitDelayStep( &(instance->output_k_1), u, reset);

    // execute differnetiator function
    instance->output = ( u - instance->output_k_1.output) / instance->time_step;

    UnitDelayPostStep( &(instance->output_k_1), u);

}