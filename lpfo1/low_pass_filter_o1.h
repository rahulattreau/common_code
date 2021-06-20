#pragma once
#include <stdbool.h>

/*
Description:
Object definition for first order low pass filter.
tau - time constant, seconds
time_step - time_step, seconds
reset - reset bit for clearing filter history
xk - new sample of filtered variable
yk_1 - filtered variable value from previous time step
yk - filtered variable value from current time step

Formula:
alpha = time_step / (tau + time_step);
y[k] = alpha * x[k] + (1 - alpha) * y[k-1]
y[k] = y[k-1] + alpha * (x[k] - y[k-1])
*/

// define the variables for the low pass filter data type
typedef struct {
    float yk_; // state variable
    float yk_1_; // state variable value from previous time step
    float alpha_; // alpha value for lpf
} low_pass_filter_o1_t;

// Class member function declarations:
// declare constructor
void LowPassFilterO1_Constructor(
    low_pass_filter_o1_t * const instance, 
    const float xk, 
    const float time_step, 
    const float tau
    );
    
// declare reset function
void LowPassFilterO1_Reset(low_pass_filter_o1_t * const instance, const float xk);
// declare step function
void LowPassFilterO1_Step(low_pass_filter_o1_t * const instance, const float xk, const bool reset);