#include "unit_delay.h"

void UnitDelay_Constructor(unit_delay_t * const instance, const float init_value) {

    UnitDelay_StoreState(instance, init_value);
    UnitDelay_PostStep = &UnitDelay_StoreState;

}

void UnitDelay_StoreState(unit_delay_t * const instance, const float xk) {

    instance->xk_stored = xk;

}

void UnitDelay_Step(unit_delay_t * const instance, const float xk, const bool reset) {
    
    if (reset) {
        UnitDelay_StoreState(instance, xk);
        instance->yk_ = instance->xk_stored;
    }
    else
        instance->yk_ = instance->xk_stored;

}