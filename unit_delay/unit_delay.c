#include "control_common_code/unit_delay.h"


// declare private functions

void UnitDelayStoreState(unit_delay_t * const instance, const float input);
bool UnitDelayResetEval(unit_delay_t * const instance, const bool reset);

// define functions

void UnitDelayInit(unit_delay_t * const instance) {

    instance->initialized = false;

}

inline void UnitDelayStoreState(unit_delay_t * const instance, const float input) {

    /*
    description:
    - stores input to input_stored
    - this is a separate function as it gets used in multiple places
    - inline to reduce the function call
    */
    
    instance->input_stored = input;

}

bool UnitDelayResetEval(unit_delay_t * const instance, const bool reset) {

    /*
    description:
    This function returns the reset value with a bit true at the first time step.
    
    1. consider the following signal:
        time step: 0 1 2 3 4 5 .....
        init_:     0 1 1 1 1 1 ..... true till end of program
        !init_:    1 0 0 0 0 0 .....
        reset:     0 0 1 0 0 0 ..... this reset is an example
    2. By ORing !init_ and reset, it creates an output that is true at the first time step, 
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

void UnitDelayStep(unit_delay_t * const instance, const float input, const bool reset) {
    
    const bool unit_delay_reset = UnitDelayResetEval(instance, reset);

    if (unit_delay_reset) {

        UnitDelayStoreState(instance, input);
        instance->output = instance->input_stored;

    }
    else
        instance->output = instance->input_stored;

}

void UnitDelayPostStep(unit_delay_t * const instance, const float input) {

    UnitDelayStoreState(instance, input);

}