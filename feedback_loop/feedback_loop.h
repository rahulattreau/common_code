#pragma once
#include "../unit_delay/unit_delay.h"

// forward declaration for virtual function table
// this is to use this type in the feedback_loop_t type
struct virtual_func_table_t;

//define data type
typedef struct {
    float yk_; // output
    struct virtual_func_table_t *_virtual_func_table_ptr_; // points to the virtual function table
    unit_delay_t yk_1_;
} feedback_loop_t;

// definition of virtual function table type
struct virtual_func_table_t {
    void (*Process)(feedback_loop_t * const, const float, const bool);
};

// define virtual function call
// this function is defined in the .c file
static inline void FeedbackLoop_Process(feedback_loop_t * const instance, const float xk, const bool reset) {
    return ( *(instance->_virtual_func_table_ptr_->Process) )(instance, xk, reset);
}

void FeedbackLoop_Constructor(feedback_loop_t * const instance);

void FeedbackLoop_Init(feedback_loop_t * const instance, float xk);

void FeedbackLoop_Step(feedback_loop_t * const instance, const float xk, const bool reset);