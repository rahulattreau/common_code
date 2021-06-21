#include "pid_controller.h"

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
float ErrorFunction(input_bus_t * const input_bus) {
    
    float error;

    if(input_bus->reset) {
        error = 0.0;
    }
    else {
        error = *(input_bus->reference) - *(input_bus->sensed_value);
    }

    return error;
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
float * ProportionalFunction(const float p_gain, const float error) {

    float p_out;

    p_out = p_gain * error;

    return &p_out;

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
i_out_bus_t* IntegralFunction(
    const float i_gain,
    const float up_sat_value,
    const float lo_sat_value,
    const float init_value,
    const float time_step,
    const float error, 
    const float reset, 
    float * const i_out_z,
    float * const pre_sat_value
    ) {

    float integrand = 0.0;
    float signum_integrand = 0.0;
    float pre_sat_dead_zone = 0.0;
    float signum_dead_zone_out = 0.0;
    float i_out = 0.0;
    i_out_bus_t i_out_bus;
    
    integrand = i_gain * error;
    signum_integrand = SignumFunction(integrand);
    pre_sat_dead_zone = DeadZone(pre_sat_value, up_sat_value, lo_sat_value);
    signum_dead_zone_out = SignumFunction(pre_sat_dead_zone);

    if ( (signum_integrand == signum_dead_zone_out) && (pre_sat_dead_zone != 0) )
        integrand = 0.0;

    if(reset) {
        i_out = init_value;
    }
    else {
        // i_out = i_out + (i_gain_integrand + bc_out) * TIME_STEP; // i.e. previous i_out + integrand
        i_out = *i_out_z + integrand * time_step; // i.e. previous i_out + integrand
    }

    // assign values to i_out_bus
    i_out_bus = (i_out_bus_t){
        .integrand = integrand,
        .signum_integrand = signum_integrand,
        .pre_sat_dead_zone = pre_sat_dead_zone,
        .signum_dead_zone_out = signum_dead_zone_out,
        .i_out = i_out
    };

    return &i_out_bus;
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


// calculate the differentiator output
float DifferentialFunction(const float sensed_value, bool controller_reset, const float d_gain, const float d_filter_tau, const float time_step) {
    float d_argument = 0.0;
    float d_out = 0.0;
    float d_argument_filtered = 0.0;
    static float d_argument_filtered_z = 0.0;

    d_argument = -1.0 * d_gain * sensed_value;
    d_argument_filtered = LpfOrder1(d_filter_tau, time_step, controller_reset, d_argument, d_argument_filtered);

    // if reset is true, reset d_argument_filtered_z. Else let d_argument_filtered_z be.
    if(controller_reset)
        d_argument_filtered_z = d_argument_filtered;
    
    d_out = (d_argument_filtered - d_argument_filtered_z) / time_step;
    d_argument_filtered_z = d_argument_filtered;
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
float SumAndSat(const float p_out, const float i_out, const float d_out, const float up_sat_value, const float lo_sat_value, const float bc_gain) {
    float pre_sat_value = 0.0;
    float post_sat_value = 0.0;
    static float bc_out = 0.0;

    // calculate pre-saturated value
    pre_sat_value = p_out + i_out + d_out;

    // calculate post-saturated value
    if (pre_sat_value > up_sat_value)
        post_sat_value = up_sat_value;
    else if(pre_sat_value < lo_sat_value)
        post_sat_value = lo_sat_value;
    else
        post_sat_value = pre_sat_value;
    
    // calculate back-calculation output for using in next time step
    bc_out = bc_gain * (post_sat_value - pre_sat_value);

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
void PidControl_Step(input_bus_t * const input_bus, output_bus_t * const output_bus) {

}