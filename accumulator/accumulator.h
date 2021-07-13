#pragma once
#include "../unit_delay/unit_delay.h"

// forward declaration for virtual function table
// this is to use this type in the accumulator_t type
struct virtual_func_table_t;

//define data type
typedef struct {
    float yk_;
    struct virtual_func_table_t *_virtual_func_table_ptr_; // points to the virtual function table
    unit_delay_t yk_1_;
} accumulator_t;

// definition of virtual function table type
struct virtual_func_table_t {
    void (*Process)(accumulator_t * const, const float, const bool);
};

// define virtual function call
// this function is defined in the .c file
static inline void Accumulator_Process(accumulator_t * const instance, const float xk, const bool reset) {
    return ( *(instance->_virtual_func_table_ptr_->Process) )(instance, xk, reset);
}

void Accumulator_Constructor(accumulator_t * const instance, const float time_step);

void Accumulator_Init(accumulator_t * const instance, float xk);

void Accumulator_Step(accumulator_t * const instance, const float xk, const bool reset);