#include "unit_delay.h"

/*
declare private functions:
Store state - private function to store internal state 
Step function - this is the public function that needs to be executed every time step
*/

void UnitDelay_Reset(unit_delay_t * const instance, const float xk);

void UnitDelay_StoreState(unit_delay_t * const instance, const float xk);

// define functions

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