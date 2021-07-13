#include "accumulator.h"

static void Accumulator_Process_(accumulator_t * const, const float, const bool);

void Accumulator_Constructor(accumulator_t * const instance, const float time_step) {
    
    // initialize the state variables
    UnitDelay_Constructor( &(instance->yk_1_) );

    // create virtual table struct
    static struct virtual_func_table_t virtual_func_table_ = {
        &Accumulator_Process_
    };
    
    // attach pointer to this struct
    instance->_virtual_func_table_ptr_ = &virtual_func_table_;
    
}

void Accumulator_Init(accumulator_t * const instance, float xk) {
    UnitDelay_Init( &(instance->yk_1_), xk );
    instance->yk_ = instance->yk_1_.yk_;
}

void Accumulator_Func(accumulator_t * const instance, const float xk, const bool reset) {

    float input = 0.0;
    
    if (!reset)
        input = xk;
        
    instance->yk_ = input + instance->yk_1_.yk_;

}

static void Accumulator_Process_(accumulator_t * const instance, const float xk, const bool reset) {

    float input = 0.0;

    // execute accumulator function
    if (!reset)
        input = xk;
        
    instance->yk_ = input + instance->yk_1_.yk_;

}

void Accumulator_Step(accumulator_t * const instance, const float xk, const bool reset) {
    
    // execute unit delay step
    UnitDelay_Step( &(instance->yk_1_), xk, reset );

    // execute process
    Accumulator_Process(instance, xk, reset);
    
    // execute unit delay post step
    UnitDelay_PostStep( &(instance->yk_1_), instance->yk_ );
}