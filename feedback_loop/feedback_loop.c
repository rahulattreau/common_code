#include "feedback_loop.h"

static void FeedbackLoop_Process_(feedback_loop_t * const, const float, const bool);

void FeedbackLoop_Constructor(feedback_loop_t * const instance) {
    
    // initialize the state variables
    UnitDelay_Constructor( &(instance->yk_1_) );

    // create virtual table struct
    static struct virtual_func_table_t virtual_func_table_ = {
        &FeedbackLoop_Process_
    };
    
    // attach pointer to this struct
    instance->_virtual_func_table_ptr_ = &virtual_func_table_;
    
}

void FeedbackLoop_Init(feedback_loop_t * const instance, float xk) {
    UnitDelay_Init( &(instance->yk_1_), xk );
    instance->yk_ = instance->yk_1_.yk_;
}

static void FeedbackLoop_Process_(feedback_loop_t * const instance, const float xk, const bool reset) {

    float input = 0.0;

    // execute feedback_loop function
    if (!reset)
        input = xk;
        
    instance->yk_ = input + instance->yk_1_.yk_;

}

void FeedbackLoop_Step(feedback_loop_t * const instance, const float xk, const bool reset) {
    
    // execute unit delay step
    UnitDelay_Step( &(instance->yk_1_), xk, reset );

    // execute process
    FeedbackLoop_Process(instance, xk, reset);
    
    // execute unit delay post step
    UnitDelay_PostStep( &(instance->yk_1_), instance->yk_ );
}