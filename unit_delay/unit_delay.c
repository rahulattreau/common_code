#include "unit_delay.h"

// declare private functions:

void UnitDelay_StoreState(unit_delay_t * const instance, const float xk);
bool UnitDelay_ResetEval(unit_delay_t * const instance, const bool reset);

// define functions

void UnitDelay_Constructor(unit_delay_t * const instance) {

    instance->init_ = false;
    // point function pointers
    UnitDelay_PostStep = &UnitDelay_StoreState;

}

inline void UnitDelay_StoreState(unit_delay_t * const instance, const float xk) {

    instance->xk_stored = xk;

}

bool UnitDelay_ResetEval(unit_delay_t * const instance, const bool reset) {

    /*
    description:
    1. This function returns the reset value with a bit true at the first time step
    2 init_ is false at the first time step and then true after
    */
    const bool unit_delay_reset = !(instance->init_) || reset;

    // make init_ true after first use
    if (instance->init_ == false)
        instance->init_ = true;

    return unit_delay_reset;

}

void UnitDelay_Step(unit_delay_t * const instance, const float xk, const bool reset) {
    
    const bool unit_delay_reset = UnitDelay_ResetEval(instance, reset);

    if (unit_delay_reset) {

        UnitDelay_StoreState(instance, xk);
        instance->yk_ = instance->xk_stored;

    }
    else
        instance->yk_ = instance->xk_stored;

}