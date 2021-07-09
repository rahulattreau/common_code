#include "unit_delay.h"

void UnitDelay_Constructor(unit_delay_t * const instance) {

    // point function pointers
    UnitDelay_Init = &UnitDelay_Reset;
    UnitDelay_PostStep = &UnitDelay_StoreState;

}

void UnitDelay_Reset(unit_delay_t * const instance, const float xk) {
    
    UnitDelay_StoreState(instance, xk);
    instance->yk_ = instance->xk_stored;

}

void UnitDelay_StoreState(unit_delay_t * const instance, const float xk) {

    instance->xk_stored = xk;

}

void UnitDelay_Step(unit_delay_t * const instance, const float xk, const bool reset) {
    
    if (reset)
        UnitDelay_Reset(instance, xk);
    else
        instance->yk_ = instance->xk_stored;

}