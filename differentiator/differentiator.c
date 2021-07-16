#include "differentiator.h"

void Differentiator_Constructor(differentiator_t * const instance, const float time_step) {

    UnitDelay_Constructor( &(instance->yk_1_) );
    instance->time_step_ = time_step;

}

void Differentiator_Step(differentiator_t * const instance, const float u, const bool reset) {

    UnitDelay_Step( &(instance->yk_1_), u, reset);

    // execute differnetiator function
    instance->yk_ = ( u - instance->yk_1_.yk_) / instance->time_step_;

    UnitDelay_PostStep( &(instance->yk_1_), u);

}