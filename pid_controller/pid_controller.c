#include <stdbool.h>
#include "pid_controller.h"
#include "../signum_function/signum_function.h"
#include "../deadzone/deadzone.h"

// ===== declare private member functions =====

// error function
void ErrorFunction(input_bus_t * const input_bus, float * const error);

// proportional function
void ProportionalFunction(float * const p_out, const float p_gain, const float error);

// integral function
void IntegralFunction(
    input_bus_t * const input_bus, 
    const float error, 
    i_out_bus_t * const i_out_bus,
    sat_and_sum_bus_t * const sat_and_sum_bus
    );

// integral clamping
void IntegralClamping(
    const float integrand, 
    const float pre_sat_value, 
    const float up_sat_value, 
    const float lo_sat_value,
    i_out_bus_t * const i_out_bus
    );

// differential function
void DifferentialFunction(input_bus_t * const input_bus, d_out_bus_t * const d_out_bus);

// ===== define functions =====

/*
Inputs:
calibration parameters:
None
dynamic values:
reference
reset
sensor values:
sensed_value

Outputs:
error
*/
// calculate error
void ErrorFunction(input_bus_t * const input_bus, float * const error) {
    
    if(input_bus->reset) {
        *error = 0.0;
    }
    else {
        *error = *(input_bus->reference) - *(input_bus->sensed_value);
    }

}

/*
inputs:
calibration parameters:
p_gain
dynamic values:
error
sensor values:
none

output:
p_out
*/
// calculate proportional output
void ProportionalFunction(float * const p_out, const float p_gain, const float error) {

    *p_out = p_gain * error;

}

/*
calibration parameters:
i_gain
up_sat_value
lo_sat_value
init_value
time_step
dynamic values:
error
reset
pre_sat_value
sensor values:
none

output:
non states:
none
state_variables:
i_out
*/

// calculate integral output
void IntegralFunction(
    input_bus_t * const input_bus, 
    const float error, 
    i_out_bus_t * const i_out_bus,
    sat_and_sum_bus_t * const sat_and_sum_bus
    ) {

    float integrand = 0.0;
    float i_out = 0.0;
    
    integrand = input_bus->i_gain * error;
    
    IntegralClamping(
    integrand, 
    sat_and_sum_bus->pre_sat_value,
    input_bus->up_sat_value, 
    input_bus->lo_sat_value,
    i_out_bus );
    
    if (i_out_bus->clamping_condition)
        integrand = 0.0;

    if(input_bus->reset) {
        i_out = input_bus->init_value;
    }
    else {
        // i_out = i_out + (i_gain_integrand + bc_out) * TIME_STEP; // i.e. previous i_out + integrand
        i_out = i_out_bus->i_out + integrand * input_bus->time_step; // i.e. previous i_out + integrand
    }

    i_out_bus->integrand = integrand;
    i_out_bus->i_out = i_out;

}

void IntegralClamping(
    const float integrand, 
    const float pre_sat_value, 
    const float up_sat_value, 
    const float lo_sat_value,
    i_out_bus_t * const i_out_bus ) {

    bool clamping_condition = false;
    const int8_t signum_integrand = SignumFunction(integrand);
    const float pre_sat_dead_zone = DeadZone(pre_sat_value, up_sat_value, lo_sat_value);
    const int8_t signum_dead_zone_out = SignumFunction(pre_sat_dead_zone);

    if ( (signum_integrand == signum_dead_zone_out) && (pre_sat_dead_zone != 0) )
        clamping_condition = true;
    
    i_out_bus->clamping_condition = clamping_condition;
    i_out_bus->signum_integrand = signum_integrand;
    i_out_bus->pre_sat_dead_zone = pre_sat_dead_zone;
    i_out_bus->signum_dead_zone_out = signum_dead_zone_out;

}

/*
inputs:
calibration parameters:
d_gain
d_filter_tau
time_step
dynamic values:
reset
sensor values:
sensed_value

outputs:
non states:
d_out
state_variables:
d_argument_filtered_z

*/

void DifferentialFunction(input_bus_t * const input_bus, d_out_bus_t * const d_out_bus) {

    float d_argument = 0.0;
    float d_out = 0.0;
    float d_argument_filtered = 0.0;
    
    d_argument = -1.0 * input_bus->d_gain * *(input_bus->sensed_value);
    
    LowPassFilterO1_Step( &(d_out_bus->d_lpf), d_argument, input_bus->reset);
    d_argument_filtered = d_out_bus->d_lpf.yk_;

    // if reset is true, reset d_argument_filtered_z. Else let d_argument_filtered_z be.
    if(input_bus->reset)
        d_out = 0.0;
    
    else
        d_out = (d_argument_filtered - d_out_bus->d_argument_filtered) / input_bus->time_step;
    
    // assign values to d_out_bus
    *d_out_bus = (d_out_bus_t) {
        .d_argument = d_argument,
        .d_out = d_out,
        .d_argument_filtered = d_argument_filtered
    };
}

/*
inputs:
calibration parameters:
up_sat_value
lo_sat_value
bc_gain
dynamic values:
reset
sensor values:
sensed_value


outputs:
pre_sat_value
post_sat_value
*/

// calculate the summer and saturation
void SumAndSat(
    float * post_sat_value,
    const float p_out, 
    const i_out_bus_t i_out_bus, 
    const d_out_bus_t d_out_bus, 
    const float up_sat_value, 
    const float lo_sat_value, 
    const float bc_gain
    ) {
    float pre_sat_value = 0.0;
    // float post_sat_value = 0.0;
    static float bc_out = 0.0;

    // calculate pre-saturated value
    pre_sat_value = p_out + i_out_bus.i_out + d_out_bus.d_out;

    // calculate post-saturated value
    if (pre_sat_value > up_sat_value)
        *post_sat_value = up_sat_value;
    else if(pre_sat_value < lo_sat_value)
        *post_sat_value = lo_sat_value;
    else
        *post_sat_value = pre_sat_value;
    
    // calculate back-calculation output for using in next time step
    bc_out = bc_gain * (*post_sat_value - pre_sat_value);

}

// define initialize pid control
void PidControl_Constructor(
    output_bus_t *output_bus,
    input_bus_t *input_bus,
    float *reference_pointer,
    float *sensed_value_pointer,
    bool *reset_pointer
    ) {
    
    float d_init_value;

    // attach pointers
    input_bus->reference = reference_pointer;
    input_bus->sensed_value = sensed_value_pointer;
    input_bus->reset = reset_pointer;

    // set differential init value
    d_init_value = - *(input_bus->sensed_value);
    // intialize differntial low pass filter
    LowPassFilterO1_Constructor(
        &(output_bus->d_out_bus.d_lpf), 
        d_init_value, 
        input_bus->time_step, 
        input_bus->d_filter_tau);
    // pass in the required variables for this

    // run the controller step function once to initialize the 
    // output values
    PidControl_Step(input_bus, output_bus);

}

// define pid function
void PidControl_Step(input_bus_t * const input_bus, output_bus_t * const output_bus) {

    // error function -> deadzone 
    // -> proportional function, integral function,differential function
    // -> sum and saturate 

    // error function
    ErrorFunction( input_bus, &(output_bus->error) );

    // deadzone
    output_bus->error = DeadZone( output_bus->error, input_bus->up_sat_value, input_bus->lo_sat_value );
    
    // proportional function
    ProportionalFunction( &(output_bus->p_out), input_bus->p_gain, output_bus->error );
    
    // integral function
    IntegralFunction(
    input_bus, 
    output_bus->error, 
    &(output_bus->i_out_bus),
    &(output_bus->sat_and_sum_out_bus)
    );
    
    // differential function
    DifferentialFunction( input_bus, &(output_bus->d_out_bus) );

    // sum and saturate
    SumAndSat(
        &(output_bus->sat_and_sum_out_bus.post_sat_value),
        output_bus->p_out, 
        output_bus->i_out_bus, 
        output_bus->d_out_bus, 
        input_bus->up_sat_value, 
        input_bus->lo_sat_value, 
        input_bus->bc_gain
    );

}