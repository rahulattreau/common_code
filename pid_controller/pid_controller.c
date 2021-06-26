#include <stdbool.h>
#include "pid_controller.h"
#include "../signum_function/signum_function.h"
#include "../deadzone/deadzone.h"

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
void ErrorFunction(float * const error, input_bus_t * const input_bus) {
    
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
    ) {

    float integrand = 0.0;
    float signum_integrand = 0.0;
    float pre_sat_dead_zone = 0.0;
    float signum_dead_zone_out = 0.0;
    float i_out = 0.0;
    bool clamping_condition = false;
    
    integrand = i_gain * error;
    
    clamping_condition = IntegralClamping(
    integrand, 
    *pre_sat_value, 
    up_sat_value, 
    lo_sat_value );
    
    if (clamping_condition)
        integrand = 0.0;

    if(reset) {
        i_out = init_value;
    }
    else {
        // i_out = i_out + (i_gain_integrand + bc_out) * TIME_STEP; // i.e. previous i_out + integrand
        i_out = *i_out_z + integrand * time_step; // i.e. previous i_out + integrand
    }

    // assign values to i_out_bus
    *i_out_bus = (i_out_bus_t){
        .integrand = integrand,
        .signum_integrand = signum_integrand,
        .pre_sat_dead_zone = pre_sat_dead_zone,
        .signum_dead_zone_out = signum_dead_zone_out,
        .clamping_condition = clamping_condition,
        .i_out = i_out
    };
}

bool IntegralClamping(
    const float integrand, 
    const float pre_sat_value, 
    const float up_sat_value, 
    const float lo_sat_value ) {

    bool clamping_condition = false;
    const int8_t signum_integrand = SignumFunction(integrand);
    const float pre_sat_dead_zone = DeadZone(pre_sat_value, up_sat_value, lo_sat_value);
    const int8_t signum_dead_zone_out = SignumFunction(pre_sat_dead_zone);

    if ( (signum_integrand == signum_dead_zone_out) && (pre_sat_dead_zone != 0) )
        clamping_condition = true;
    
    return clamping_condition;
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

d_out_bus_t DifferentialFunction(
    d_out_bus_t * const d_out_bus,
    const float d_gain, 
    const float d_filter_tau, 
    const float time_step, 
    bool controller_reset, 
    const float sensed_value,
    float * const d_argument_filtered_z) {

    float d_argument = 0.0;
    float d_out = 0.0;
    float d_argument_filtered = 0.0;
    
    d_argument = -1.0 * d_gain * sensed_value;
    d_argument_filtered = LpfOrder1(d_filter_tau, time_step, controller_reset, d_argument, *d_argument_filtered_z);

    // if reset is true, reset d_argument_filtered_z. Else let d_argument_filtered_z be.
    if(controller_reset)
        d_out = 0.0;
    
    else
        d_out = (d_argument_filtered - *d_argument_filtered_z) / time_step;
    
    // assign values to d_out_bus
    *d_out_bus = (d_out_bus_t) {
        .d_argument = d_argument,
        .d_out = d_out,
        .d_argument_filtered = d_argument_filtered,
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
    input_bus_t *_input_bus,
    float *reference_pointer,
    float *sensed_value_pointer,
    bool *reset_pointer
    ) {

    // attach pointers
    _input_bus->reference = reference_pointer;
    _input_bus->sensed_value = sensed_value_pointer;
    _input_bus->reset = reset_pointer;

    // set ouputs to default values
    // ...

}

// define pid function
void PidControl_Step(parameters_t * const parameters, input_bus_t * const input_bus, output_bus_t * const output_bus) {

    // error function -> deadzone 
    // -> proportional function, integral function,differential function
    // -> sum and saturate 

    // error function
    ErrorFunction( &(output_bus->error), input_bus );

    // deadzone
    output_bus->error = DeadZone( output_bus->error, parameters->up_sat_value, parameters->lo_sat_value );
    
    // proportional function
    ProportionalFunction( &(output_bus->p_out), parameters->p_gain, output_bus->error );
    
    // integral function
    IntegralFunction(
    &(output_bus->i_out_bus),
    parameters->i_gain,
    parameters->up_sat_value,
    parameters->lo_sat_value,
    parameters->init_value,
    parameters->time_step,
    output_bus->error, 
    input_bus->reset, 
    &(output_bus->i_out_bus.i_out),
    &(output_bus->sat_and_sum_out_bus.pre_sat_value)
    );
    
    // differential function
    DifferentialFunction(
        &(output_bus->d_out_bus),
        parameters->d_gain, 
        parameters->d_filter_tau, 
        parameters->time_step, 
        input_bus->reset, 
        input_bus->sensed_value,
        &(output_bus->d_out_bus.d_argument_filtered)
        );
    
    // sum and saturate
    SumAndSat(
        &(output_bus->sat_and_sum_out_bus.post_sat_value),
        output_bus->p_out, 
        output_bus->i_out_bus, 
        output_bus->d_out_bus, 
        parameters->up_sat_value, 
        parameters->lo_sat_value, 
        parameters->bc_gain
    ); 

    }