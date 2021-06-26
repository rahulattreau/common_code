#include <stdbool.h>

#ifdef __INT8_TYPE__
typedef __INT8_TYPE__ int8_t;
#endif

// define integral output data type:
typedef struct {
    float integrand;
    float signum_integrand;
    float pre_sat_dead_zone;
    float signum_dead_zone_out;
    bool clamping_condition;
    float i_out;
} i_out_bus_t;

// define differential output data type
typedef struct {
    float d_argument;
    float d_out;
    float d_argument_filtered;
} d_out_bus_t;

// define sum and sat output data type:
typedef struct {
    float pre_sat_value;
    float post_sat_value;
    float bc_out;
} sat_and_sum_bus_t;

// ===== controller level data types =====

// define parameter struct data type
typedef struct {
    float p_gain;
    float i_gain;
    float d_gain;
    float bc_gain;
    float d_filter_tau;
    float up_sat_value;
    float lo_sat_value;
    float init_value;
    float time_step;
    
} parameters_t;

// define input bus data type
typedef struct {
    float *reference;
    float *sensed_value;
    float *reset;
} input_bus_t;

// define output bus data type
typedef struct {
    float error;
    float p_out;
    i_out_bus_t i_out_bus;
    d_out_bus_t d_out_bus;
    sat_and_sum_bus_t sat_and_sum_out_bus;
} output_bus_t;

// ===== declare public member functions =====

void PidControl_Constructor(
    input_bus_t *_input_bus,
    float *reference_pointer,
    float *sensed_value_pointer,
    bool *reset_pointer
    );

// ===== declare private member functions =====

// error function
void ErrorFunction(float * const error, input_bus_t * const input_bus);

// proportional function
void ProportionalFunction(float * const p_out, const float p_gain, const float error);

// integral function
void IntegralFunction(
    i_out_bus_t * const i_out_bus,
    const float i_gain,
    const float up_sat_value,
    const float lo_sat_value,
    const float init_value,
    const float time_step,
    const float error, 
    const bool reset, 
    float * const i_out_z,
    float * const pre_sat_value
    );

// integral clamping
bool IntegralClamping(
    const float integrand, 
    const float pre_sat_value, 
    const float up_sat_value, 
    const float lo_sat_value );

// differential function
d_out_bus_t DifferentialFunction(
    d_out_bus_t * const d_out_bus,
    const float d_gain, 
    const float d_filter_tau, 
    const float time_step, 
    bool controller_reset, 
    const float sensed_value,
    float * const d_argument_filtered_z);