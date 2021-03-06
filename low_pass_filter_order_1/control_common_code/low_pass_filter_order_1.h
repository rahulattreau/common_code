#ifndef LOW_PASS_FILTER_ORDER_1
#define LOW_PASS_FILTER_ORDER_1

#include <stdbool.h>
#include "../../unit_delay/control_common_code/unit_delay.h"

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
*/

// define data type
typedef struct {
    float output; // output variable
    unit_delay_t output_k_1; // unit delay object
    float alpha; // alpha value for lpf
} low_pass_filter_o1_t;

// Class member function declarations:

/*
declare constructor
    - use when declaring variable
*/
void LowPassFilterOrder1Init(
    low_pass_filter_o1_t * const instance, 
    const float time_step, 
    const float tau
    );

/*
declare step function
    - this is the public function that needs to run in each time step
*/
void LowPassFilterOrder1Step(low_pass_filter_o1_t * const instance, const float input, const bool reset);

#endif // LOW_PASS_FILTER_ORDER_1