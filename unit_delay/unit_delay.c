#include "unit_delay.h"

void UnitDelay_Constructor(unit_delay_t * const instance, const float init_value) {
    const bool reset = true; 
    UnitDelay_Step(instance, init_value, reset);
}

void UnitDelay_Step(unit_delay_t * const instance, const float xk, const bool reset) {
    
    if (reset) {
        instance->xk_stored = xk;
        instance->yk_ = instance->xk_stored;
    }
    else
    {
        instance->yk_ = instance->xk_stored;
        instance->xk_stored = xk;
    }

}