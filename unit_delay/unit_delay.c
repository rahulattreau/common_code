#include "control_common_code/unit_delay.h"


// declare private functions

void UnitDelay_StoreState(unit_delay_t * const instance, const float input);
bool UnitDelay_ResetEval(unit_delay_t * const instance, const bool reset);

// define functions

void UnitDelay_Constructor(unit_delay_t * const instance) {

    instance->initialized = false;

}

inline void UnitDelay_StoreState(unit_delay_t * const instance, const float input) {

    /*
    description:
    - stores input to input_stored
    - this is a separate function as it gets used in multiple places
    - inline to reduce the function call
    */
    
    instance->input_stored = input;

}

bool UnitDelay_ResetEval(unit_delay_t * const instance, const bool reset) {

    /*
    description:
    This function returns the reset value with a bit true at the first time step.
    
    1. consider the following signal:
        time step: 0 1 2 3 4 5 .....
        initialized:     0 1 1 1 1 1 ..... true till end of program
        !initialized:    1 0 0 0 0 0 .....
        reset:     0 0 1 0 0 0 ..... this reset is an example
    2. By ORing !initialized and reset, it creates an output that is true at the first time step, 
        and equal to reset in all subsequent time steps
    3. this ensures that the unit delay gets initialized in the first time step of the program,
        as there is no 'previous time step' for the unit delay to get data from

    */
    const bool unit_delay_reset = !(instance->initialized) || reset;

    // make initialized true after first use
    if (instance->initialized == false)
        instance->initialized = true;

    return unit_delay_reset;

}

void UnitDelay_Step(unit_delay_t * const instance, const float input, const bool reset) {
    
    const bool unit_delay_reset = UnitDelay_ResetEval(instance, reset);

    if (unit_delay_reset) {

        UnitDelay_StoreState(instance, input);
        instance->output = instance->input_stored;

    }
    else
        instance->output = instance->input_stored;

}

void UnitDelay_PostStep(unit_delay_t * const instance, const float input) {

    UnitDelay_StoreState(instance, input);

}