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

// sum and sat function
void SumAndSat(
    float * post_sat_value,
    const float p_out, 
    const i_out_bus_t i_out_bus, 
    const d_out_bus_t d_out_bus, 
    const float up_sat_value, 
    const float lo_sat_value, 
    const float bc_gain
    );

// ===== define functions =====

// calculate error
void ErrorFunction(input_bus_t * const input_bus, float * const error) {
    
    /*
    description:
    1. if reset is true, set error to zero. This is so that the control output is zero
    2. Else, calculate error
    */
   
    if(input_bus->reset) {
        *error = 0.0;
    }
    else {
        *error = input_bus->reference - *(input_bus->sensed_value);
    }

}

// calculate proportional output
void ProportionalFunction(float * const p_out, const float p_gain, const float error) {

    *p_out = p_gain * error;

}

// calculate integral output
void IntegralFunction(
    input_bus_t * const input_bus, 
    const float error, 
    i_out_bus_t * const i_out_bus,
    sat_and_sum_bus_t * const sat_and_sum_bus
    ) {
    
    /*
    description:
    1. calculate integrand
    2. calculate clamping condition
    3. reset integrator if required
    4. set outputs
    */

    float integrand = 0.0;
    
    integrand = input_bus->i_gain * error;
    
    IntegralClamping(
    integrand, 
    sat_and_sum_bus->pre_sat_value,
    input_bus->up_sat_value, 
    input_bus->lo_sat_value,
    i_out_bus );
    
    if (i_out_bus->clamping_condition)
        integrand = 0.0;

    Integrator_Step( &(i_out_bus->integrator), integrand, input_bus->init_value, input_bus->reset);

    i_out_bus->integrand = integrand;
    i_out_bus->i_out = i_out_bus->integrator.accumulator.yk_;

}

// integral clamping definition
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

// define differential function
void DifferentialFunction(input_bus_t * const input_bus, d_out_bus_t * const d_out_bus) {

    /*
    description:
    1. filter the argument. The argument is the sensed value
    2. reset if required.
    3. calculate differential out
    4. set outputs
    */

    float d_argument = 0.0;
    float d_out = 0.0;
    float d_argument_filtered = 0.0;
    float *d_argument_filtered_z = &(d_out_bus->d_argument_filtered);
    
    d_argument = *(input_bus->sensed_value);
    
    LowPassFilterO1_Step( &(d_out_bus->d_lpf), d_argument, input_bus->reset);
    
    d_argument_filtered = d_out_bus->d_lpf.yk_;
    
    // if reset, then reset unit delay
    if(input_bus->reset)
        *d_argument_filtered_z = d_argument_filtered;
    
    d_out = -1.0 * input_bus->d_gain * (d_argument_filtered - *d_argument_filtered_z) / input_bus->time_step;

    // assign values to d_out_bus
    d_out_bus->d_argument = *(input_bus->sensed_value);
    d_out_bus->d_out = d_out;
    d_out_bus->d_argument_filtered = d_argument_filtered;
}

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
    
    /*
    description:
    1. sum p_out, i_out and d_out
    2. saturate
    */
    
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
    float *sensed_value_pointer
    ) {

    // attach pointers
    input_bus->sensed_value = sensed_value_pointer;
    
    // call integrator constructor
    Integrator_Constructor( &(output_bus->i_out_bus.integrator), input_bus->time_step);

    // call differential low pass filter constructor
    LowPassFilterO1_Constructor(
        &(output_bus->d_out_bus.d_lpf), 
        input_bus->time_step, 
        input_bus->d_filter_tau);
    
}

// define pid function
void PidControl_Step(output_bus_t * const output_bus, input_bus_t * const input_bus) {

    // description:
    // error function -> deadzone -> proportional function, integral function, differential function -> sum and saturate 

    // error function
    ErrorFunction( input_bus, &(output_bus->error) );

    // deadzone
    output_bus->error = DeadZone( output_bus->error, input_bus->dead_zone_up, input_bus->dead_zone_lo );
    
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