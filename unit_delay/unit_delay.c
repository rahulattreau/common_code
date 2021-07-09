#include "unit_delay.h"

void UnitDelay_Constructor(unit_delay_t * const instance, const float init_value) {
    const bool reset = true; 
    UnitDelay_Step(instance, init_value, reset);
}

void UnitDelay_StoreInternalState(unit_delay_t * const instance, const float xk) {
    instance->xk_1_ = xk;
}

void UnitDelay_Step(unit_delay_t * const instance, const float xk, const bool reset) {
    
    // if (reset) {
    //     UnitDelay_StoreInternalState(instance, xk);
    //     instance->yk_ = instance->xk_1_;
    // }
    // else
    // {
    //     instance->yk_ = instance->xk_1_;
    //     UnitDelay_StoreInternalState(instance, xk);
    // }
    if (reset) {
        instance->xk_1_ = xk;
        instance->yk_ = instance->xk_1_;
    }
    else
    {
        instance->yk_ = instance->xk_1_;
        instance->xk_1_ = xk;
    }

}