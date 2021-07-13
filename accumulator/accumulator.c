#include "accumulator.h"

// local declaration of virtual function
static void Accumulator_Process_(feedback_loop_t * const instance, const float xk, const bool reset);

// constructor
void Accumulator_Constructor(accumulator_t * const instance) {

    FeedbackLoop_Constructor( &(instance->loop) );

    // create virtual table struct
    static struct virtual_func_table_t virtual_func_table_ = {
        &Accumulator_Process_
    };
    
    // attach pointer to this struct
    instance->loop._virtual_func_table_ptr_ = &virtual_func_table_;

    // attach function pointers
    Accumulator_Init = &FeedbackLoop_Init;
    Accumulator_Step = &FeedbackLoop_Step;

}

// local definition of virtual function
static void Accumulator_Process_(feedback_loop_t * const instance, const float xk, const bool reset) {

    float input = 0.0;

    if (!reset)
        input = xk;
        
    instance->yk_ = input + instance->yk_1_.yk_;

}