#include <stdbool.h>
#include "control_common_code/pid_controller.h"
#include "../signum_function/control_common_code/signum_function.h"
#include "../deadzone/control_common_code/deadzone.h"
#include "../saturator/saturator.h"

// ===== declare private member functions =====

// error function
void ErrorFunction(float * const error, input_bus_t * const input_bus);

// proportional function
void ProportionalFunction(float * const p_out, const float p_gain, const float error);

// integral function
void IntegralFunction(
    i_out_bus_t * const i_out_bus,
    sat_and_sum_bus_t * const sat_and_sum_bus,
    const float error, 
    input_bus_t * const input_bus    
    );

// integral clamping
void IntegralClamping(
    i_out_bus_t * const i_out_bus,
    const float integrand, 
    const float pre_sat_value, 
    const float up_sat_value, 
    const float lo_sat_value
    );

// differential function
void DifferentialFunction(d_out_bus_t * const d_out_bus, input_bus_t * const input_bus);

// sum and sat function
void SumAndSat(pid_control_bus_t * const instance, input_bus_t * const input_bus);

// ===== define functions =====

// calculate error
void ErrorFunction(float * const error, input_bus_t * const input_bus) {
    
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
void ProportionalFunction(float * const p_out, const float p_gain, const float error) {

    *p_out = p_gain * error;

}

// calculate integral output
void IntegralFunction(
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
    
    IntegralClamping(
        i_out_bus,
        i_out_bus->integrand, 
        sat_and_sum_bus->pre_sat_value_k_1_.yk_,
        input_bus->up_sat_value, 
        input_bus->lo_sat_value
        );
    
    if (i_out_bus->clamping_condition)
        i_out_bus->integrand = 0.0;

    Integrator_Step( &(i_out_bus->integrator), i_out_bus->integrand, input_bus->init_value, input_bus->reset);
    
}

// integral clamping definition
void IntegralClamping(
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
void DifferentialFunction(d_out_bus_t * const d_out_bus, input_bus_t * const input_bus) {

    /*
    description:
    1. filter the argument. The argument is the sensed value
    2. reset if required.
    3. calculate differential out
    4. set outputs
    */

    const float d_argument = -1.0 * *(input_bus->sensed_value);
    LowPassFilterO1_Step( &(d_out_bus->d_argument_filtered), d_argument, input_bus->reset);
    Differentiator_Step( &(d_out_bus->differentiator), d_out_bus->d_argument_filtered.yk_, input_bus->reset );

}

// calculate the summer and saturation
void SumAndSat(pid_control_bus_t * const instance, input_bus_t * const input_bus) {
    
    /*
    description:
    1. run pre_sat_value unit delay step function
    2. calculate pre_sat_value - sum p_out, i_out and d_out
    3. calculate post_sat_value - saturate
    4. run post_sat_value unit delay step function
    */
    
    // creating local pointer to reduce variable lengths
    sat_and_sum_bus_t * const _sat_and_sum_out_bus = &(instance->sat_and_sum_out_bus);
    
    UnitDelay_Step( &(_sat_and_sum_out_bus->pre_sat_value_k_1_), 
        input_bus->init_value, input_bus->reset);

    _sat_and_sum_out_bus->pre_sat_value = instance->p_out 
        + instance->i_out_bus.integrator.yk_ 
        + instance->d_out_bus.differentiator.yk_;
    _sat_and_sum_out_bus->post_sat_value = Saturator(
        _sat_and_sum_out_bus->pre_sat_value, 
        input_bus->up_sat_value, 
        input_bus->lo_sat_value);
    
    UnitDelay_PostStep( &(_sat_and_sum_out_bus->pre_sat_value_k_1_), _sat_and_sum_out_bus->pre_sat_value);

}

// define initialize pid control
void PidControl_Constructor(pid_control_bus_t *output_bus, input_bus_t *input_bus, float *sensed_value) {
    
    // hook the input sensor (sensed_value) to the to input_bus->sensed_value 
    input_bus->sensed_value = sensed_value;

    // integral function constructors
    Integrator_Constructor( &(output_bus->i_out_bus.integrator), input_bus->time_step);

    // call differential function constructors
    LowPassFilterO1_Constructor(
        &(output_bus->d_out_bus.d_argument_filtered), 
        input_bus->time_step, 
        input_bus->d_filter_tau);
    Differentiator_Constructor( &(output_bus->d_out_bus.differentiator), input_bus->time_step );

    // call sat and sum bus function constructors
    UnitDelay_Constructor( &(output_bus->sat_and_sum_out_bus.pre_sat_value_k_1_) );
    
}

// define pid function
void PidControl_Step(pid_control_bus_t * const pid_control_bus, input_bus_t * const input_bus) {

    /* 
    description:
    error function -> deadzone -> proportional function, integral function, differential function -> 
        sum and saturate 
    */

    ErrorFunction( &(pid_control_bus->error), input_bus );
    pid_control_bus->error = DeadZone( pid_control_bus->error, input_bus->dead_zone_up, input_bus->dead_zone_lo );
    ProportionalFunction( &(pid_control_bus->p_out), input_bus->p_gain, pid_control_bus->error );
    IntegralFunction(
        &(pid_control_bus->i_out_bus),
        &(pid_control_bus->sat_and_sum_out_bus),
        pid_control_bus->error, 
        input_bus        
        );
    DifferentialFunction( &(pid_control_bus->d_out_bus), input_bus );
    SumAndSat( pid_control_bus, input_bus );
    
}