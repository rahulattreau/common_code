#include "integrator.h"

// local declaration of virtual function
static void Integrator_Process_(feedback_loop_t * const instance, const float xk, const bool reset);

// constructor
void Integrator_Constructor(integrator_t * const instance, const float time_step) {

    FeedbackLoop_Constructor( &(instance->loop_) );

    // create virtual table struct
    static struct virtual_func_table_t virtual_func_table_ = {
        &Integrator_Process_
    };
    
    // attach pointer to this struct
    instance->loop_._virtual_func_table_ptr_ = &virtual_func_table_;

    // attach function pointers
    Integrator_Init = &FeedbackLoop_Init;
    Integrator_Step = &FeedbackLoop_Step;

    //
    instance->time_step_ = time_step;

}

// local definition of virtual function
static void Integrator_Process_(feedback_loop_t * const instance, const float xk, const bool reset) {

    float input = 0.0;

    if (!reset)
        input = xk;
        
    instance->yk_ = input + instance->yk_1_.yk_;

}