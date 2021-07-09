#include "unit_delay.h"

void UnitDelay_Constructor(unit_delay_t * const instance, const float init_value) {
    const bool reset = true; 
    UnitDelay_Step(instance, init_value, reset);
}

void UnitDelay_StoreInternalState(unit_delay_t * const instance, const float xk) {
    instance->yk_1_.value_ = xk;
}

void UnitDelay_Step(unit_delay_t * const instance, const float xk, const bool reset) {
    
    // if (reset) {
    //     UnitDelay_StoreInternalState(instance, xk);
    //     instance->yk_ = instance->yk_1_.value_;
    // }
    // else
    // {
    //     instance->yk_ = instance->yk_1_.value_;
    //     UnitDelay_StoreInternalState(instance, xk);
    // }
    if (reset) {
        instance->yk_1_.value_ = xk;
        instance->yk_ = instance->yk_1_.value_;
    }
    else
    {
        instance->yk_ = instance->yk_1_.value_;
        instance->yk_1_.value_ = xk;
    }

}