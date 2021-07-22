#include <stdbool.h>
#include "control_common_code/pid_controller.h"
#include "../signum_function/control_common_code/signum_function.h"
#include "../deadzone/control_common_code/deadzone.h"
#include "../saturator/saturator.h"

// ===== declare private member functions =====

// error function
void PidControl_ErrorFunction(float * const error, input_bus_t * const input_bus);

// proportional function
void PidControl_ProportionalFunction(float * const p_out, const float p_gain, const float error);

// integral function
void PidControl_IntegralFunction(
    i_out_bus_t * const i_out_bus,
    sat_and_sum_bus_t * const sat_and_sum_bus,
    const float error, 
    input_bus_t * const input_bus    
    );

// integral clamping
void PidControl_IntegralClamping(
    i_out_bus_t * const i_out_bus,
    const float integrand, 
    const float pre_sat_value, 
    const float up_sat_value, 
    const float lo_sat_value
    );

// differential function
void PidControl_DifferentialFunction(d_out_bus_t * const d_out_bus, input_bus_t * const input_bus);

// sum and sat function
void PidControl_SumAndSat(pid_control_bus_t * const instance, input_bus_t * const input_bus);

// ===== define functions =====

// calculate error
void PidControl_ErrorFunction(float * const error, input_bus_t * const input_bus) {
    
    /*
    description:
    1. if reset is true, set error to zero. This is so that the control output is zero
    2. Else, calculate error
    */
   
    if(input_bus->reset)
        *error = 0.0;
    else
        *error = input_bus->reference - *(input_bus->sensed_value);

}

// calculate proportional output
void PidControl_ProportionalFunction(float * const p_out, const float p_gain, const float error) {

    *p_out = p_gain * error;

}

// calculate integral output
void PidControl_IntegralFunction(
    i_out_bus_t * const i_out_bus,
    sat_and_sum_bus_t * const sat_and_sum_bus,
    const float error, 
    input_bus_t * const input_bus    
    ) {
    
    /*
    description:
    1. calculate integrand
    2. calculate clamping condition and set integrand value to zero if clamping condition is true
    3. call integrator step
    */

    i_out_bus->integrand = input_bus->i_gain * error;
    
    PidControl_IntegralClamping(
        i_out_bus,
        i_out_bus->integrand, 
        sat_and_sum_bus->pre_sat_value_k_1.output,
        input_bus->up_sat_value, 
        input_bus->lo_sat_value
        );
    
    if (i_out_bus->clamping_condition)
        i_out_bus->integrand = 0.0;

    IntegratorStep( &(i_out_bus->integrator), i_out_bus->integrand, input_bus->init_value, input_bus->reset);
    
}

// integral clamping definition
void PidControl_IntegralClamping(
    i_out_bus_t * const i_out_bus,
    const float integrand, 
    const float pre_sat_value, 
    const float up_sat_value, 
    const float lo_sat_value
    ) {

    /*
    description:
    1. This function calculates a clamping condition, meaning when the integrator needs to be clamped for anti-windup
    2. Get the signum function output of the integrand.
    3. On the pre-saturated value, perform deadzone->signum.
        - the deadzone will:
            - give a value of zero if the pre_sat value is WITHIN range
            - positive value if the pre-sat is above the upper saturation value
            - negative value if the pre-sat is below the lower saturation value
        - the signum function will, give a value of [-1,0,1] depending on the sign of the output of the pre_sat 
            deadzone
        - when the pre_sat_dead_zone is out of range, it's value is non-zero.
    4. If pre_sat_dead_zone != 0, i.e. the pre-sat value is out of range, allow the integrator to operate only if 
        the integrand can bring the value back in range. For that, the sign of the integrand should not be equal to 
        the sign of the presat deadzone. Therefore:
        - if the signs are equal, when the presat value is out of range, set clamping to true.
    */

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
void PidControl_DifferentialFunction(d_out_bus_t * const d_out_bus, input_bus_t * const input_bus) {

    /*
    description:
    1. filter the argument. The argument is the sensed value
    2. reset if required.
    3. calculate differential out
    4. set outputs
    */

    const float d_argument = -1.0 * *(input_bus->sensed_value);
    LowPassFilterOrder1Step( &(d_out_bus->d_argument_filtered), d_argument, input_bus->reset);
    DifferentiatorStep( &(d_out_bus->differentiator), d_out_bus->d_argument_filtered.output, input_bus->reset );

}

// calculate the summer and saturation
void PidControl_SumAndSat(pid_control_bus_t * const instance, input_bus_t * const input_bus) {
    
    /*
    description:
    1. calculate pre_sat_value - sum p_out, i_out and d_out
    2. calculate post_sat_value - saturate
    */
    
    // creating local pointer to reduce variable lengths
    sat_and_sum_bus_t * const _sat_and_sum_out_bus = &(instance->sat_and_sum_out_bus);
    
    _sat_and_sum_out_bus->pre_sat_value = instance->p_out 
        + instance->i_out_bus.integrator.output 
        + instance->d_out_bus.differentiator.output;
    _sat_and_sum_out_bus->post_sat_value = Saturator(
        _sat_and_sum_out_bus->pre_sat_value, 
        input_bus->up_sat_value, 
        input_bus->lo_sat_value);

}

// define initialize pid control
void PidControlInit(pid_control_bus_t *output_bus, input_bus_t *input_bus, float *sensed_value) {
    
    // hook the input sensor (sensed_value) to the to input_bus->sensed_value 
    input_bus->sensed_value = sensed_value;

    // integral function constructors
    IntegratorInit( &(output_bus->i_out_bus.integrator), input_bus->time_step);

    // call differential function constructors
    LowPassFilterOrder1Init(
        &(output_bus->d_out_bus.d_argument_filtered), 
        input_bus->time_step, 
        input_bus->d_filter_tau);
    DifferentiatorInit( &(output_bus->d_out_bus.differentiator), input_bus->time_step );

    // call sat and sum bus function constructors
    UnitDelayInit( &(output_bus->sat_and_sum_out_bus.pre_sat_value_k_1) );
    
}

// define pid function
void PidControlStep(pid_control_bus_t * const instance, input_bus_t * const input_bus) {

    /* 
    description:
    1. Run state functions
    2. error function -> deadzone -> proportional function, integral function, differential function -> 
        sum and saturate -> sat value to output output
    3. Run post step function
    */
    
    UnitDelayStep( &(instance->sat_and_sum_out_bus.pre_sat_value_k_1), input_bus->init_value, input_bus->reset);

    PidControl_ErrorFunction( &(instance->error), input_bus );
    instance->error = DeadZone( instance->error, input_bus->dead_zone_up, input_bus->dead_zone_lo );
    PidControl_ProportionalFunction( &(instance->p_out), input_bus->p_gain, instance->error );
    PidControl_IntegralFunction(
        &(instance->i_out_bus),
        &(instance->sat_and_sum_out_bus),
        instance->error, 
        input_bus        
        );
    PidControl_DifferentialFunction( &(instance->d_out_bus), input_bus );
    PidControl_SumAndSat( instance, input_bus );
    instance->output = instance->sat_and_sum_out_bus.post_sat_value;

    UnitDelayPostStep( &(instance->sat_and_sum_out_bus.pre_sat_value_k_1), instance->sat_and_sum_out_bus.pre_sat_value);
    
}