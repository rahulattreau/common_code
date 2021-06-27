#include <stdbool.h>
#include "../lpfo1/low_pass_filter_o1.h"

// definition of int8_t in firmware code
// typedef __int8_t int8_t;
// this defines int8_t when outside the firmware code
#ifndef __int8_t
typedef signed char int8_t;
#endif

// define integral output data type:
typedef struct {
    float integrand;
    int8_t signum_integrand;
    float pre_sat_dead_zone;
    int8_t signum_dead_zone_out;
    bool clamping_condition;
    float i_out;
} i_out_bus_t;

// define differential output data type
typedef struct {
    float d_argument;
    float d_out;
    float d_argument_filtered;
    low_pass_filter_o1_t d_lpf;
} d_out_bus_t;

// define sum and sat output data type:
typedef struct {
    float pre_sat_value;
    float post_sat_value;
    float bc_out;
} sat_and_sum_bus_t;

// ===== controller level data types =====

// define input bus data type
typedef struct {
    float *reference;
    float *sensed_value;
    bool *reset;
    float p_gain;
    float i_gain;
    float d_gain;
    float bc_gain;
    float d_filter_tau;
    float up_sat_value;
    float lo_sat_value;
    float init_value;
    const float time_step;
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

void PidControl_Step(input_bus_t * const input_bus, output_bus_t * const output_bus);