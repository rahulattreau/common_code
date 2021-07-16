#include "control_common_code/unit_delay.h"


// declare private functions

void UnitDelay_StoreState(unit_delay_t * const instance, const float u);
bool UnitDelay_ResetEval(unit_delay_t * const instance, const bool reset);

// define functions

void UnitDelay_Constructor(unit_delay_t * const instance) {

    ResetManager_Constructor( &(instance->reset_manager) );

}

inline void UnitDelay_StoreState(unit_delay_t * const instance, const float u) {

    /*
    description:
    - stores u to u_stored
    - this is a separate function as it gets used in multiple places
    - inline to reduce the function call
    */
    
    instance->u_stored = u;

}

void UnitDelay_Step(unit_delay_t * const instance, const float u, const bool reset) {
    
    ResetManager_Step( &(instance->reset_manager), reset);

    if (instance->reset_manager.reset_state_) {

        UnitDelay_StoreState(instance, u);
        instance->yk_ = instance->u_stored;

    }
    else
        instance->yk_ = instance->u_stored;

}

void UnitDelay_PostStep(unit_delay_t * const instance, const float u) {

    UnitDelay_StoreState(instance, u);

}