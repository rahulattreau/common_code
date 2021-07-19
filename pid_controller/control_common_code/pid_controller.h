/*
Description:
1. PID control class
2. Contains no private variables
3. Has only public variables that are passed in by reference to functions
 so that the class is reusable

Data handling:
1. For output structs, a pointer to struct is passed to functions
2. Input structs are passed by pointer to functions to reduce the
 amount of data transferred
3. Input scalars are passed by value to functions

*/

#ifndef PID_CONTROLLER_H
#define PID_CONTROLLER_H

#include <stdbool.h>
#include "../lpfo1/control_common_code/low_pass_filter_o1.h"
#include "../integrator/control_common_code/integrator.h"
#include "../differentiator/differentiator.h"
#include "../unit_delay/control_common_code/unit_delay.h"
#include "../deadzone/control_common_code/deadzone.h"

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
    integrator_t integrator;
} i_out_bus_t;

// define differential output data type
typedef struct {
    float d_argument;
    low_pass_filter_o1_t d_argument_filtered;
    differentiator_t differentiator;
} d_out_bus_t;

// define sum and sat output data type:
typedef struct {
    float pre_sat_value;
    unit_delay_t pre_sat_value_k_1_;
    float post_sat_value;
    float bc_out;
} sat_and_sum_bus_t;

// ===== controller level data types =====

// define input bus data type
typedef struct {
    float reference;
    float sensed_value;
    bool reset;
    float p_gain;
    float i_gain;
    float d_gain;
    float bc_gain;
    float d_filter_tau;
    float dead_zone_up;
    float dead_zone_lo;
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
} pid_control_bus_t;

// ===== declare public member functions =====

void PidControl_Constructor(
    pid_control_bus_t *output_bus,
    input_bus_t *input_bus
    );

void PidControl_Step(pid_control_bus_t * const output_bus, input_bus_t * const input_bus);

#endif // PID_CONTROLLER_H