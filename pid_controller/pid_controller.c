// break them into constant inputs like time_step and calibration inputs
// check out which variables need to be static

// float sh_error = 0.0;
// float p_out = 0.0;
// static float i_out = EXV_POSITION_MIN;
// float i_gain_integrand = 0.0;
// float d_out = 0.0;
// static float bc_out = 0.0;
// float pre_sat_value = 0.0;
// float post_sat_value = 0.0;
// float d_argument = 0.0;
// float d_argument_filtered = 0.0;
// static float d_argument_filtered_z = 0.0;
// static uint16_t superheat_counter = 1;
// static float superheat_reference = 14.0;
// static uint16_t print_counter = 0;
// static float time_value = 0.0;
// float pre_sat_dead_zone = 0.0;
// const float diminishing_error_band = 3.0;
// static superheat_window_t superheat_window_values = {0.0, 0.0, false};


// end of creating controller-level data types

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
float ErrorFunction(float * const reference, float * const sensed_value, bool reset) {
    
    float error = 0.0;

    if(reset) {
        error = 0.0;
    }
    else {
        error = *reference - *sensed_value;
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
float ProportionalFunction(const float error, const float p_gain) {
    float p_out = p_gain * error;
    
    return p_out;
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

// define integral function
float IntegralFunction(const float error, bool reset, const float pre_sat_value, const float i_gain, const float up_sat_value, const float lo_sat_value, const float init_value, const float time_step) {
    float integrand = 0.0;
    float signum_integrand = 0.0;
    float pre_sat_dead_zone = 0.0;
    float signum_dead_zone_out = 0.0;
    static float i_out = 0.0;

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
        i_out = i_out + integrand * time_step; // i.e. previous i_out + integrand
    }
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

// // this is the error function
//         if(controller_reset) {
//             sh_error = 0.0;
//         }
//         else {
//             superheat_reference = SUPERHEAT_TARGET_C;
//             sh_error = superheat_reference - superheat_filtered;
//             // sh_error = superheat_reference - superheat_filtered;
//         }

//         // put a deadzone around superheat
//         sh_error = DeadZone(sh_error, 1.0, -2.0);

//         // calculate the proportional output
//         p_out = P_GAIN * sh_error;

//         // calculate the integrator output
//         i_gain_integrand = I_GAIN * sh_error;
//         signum_i_gain_integrand = SignumFunction(i_gain_integrand);
//         pre_sat_dead_zone = DeadZone(pre_sat_value, EXV_POSITION_MAX, EXV_POSITION_MIN);
//         signum_dead_zone_out = SignumFunction(pre_sat_dead_zone);

//         if ( (signum_i_gain_integrand == signum_dead_zone_out) && (pre_sat_dead_zone != 0) )
//             i_gain_integrand = 0.0;

//         if(controller_reset) {
//             i_out = EXV_INIT_POS;
//         }
//         else {
//             // i_out = i_out + (i_gain_integrand + bc_out) * TIME_STEP; // i.e. previous i_out + integrand
//             i_out = i_out + i_gain_integrand * TIME_STEP; // i.e. previous i_out + integrand
//         }

//         // calculate the differentiator output
//         d_argument = -1.0 * D_GAIN * superheat_filtered;
//         d_argument_filtered = LpfOrder1(D_FILTER_TAU, TIME_STEP, controller_reset, d_argument, d_argument_filtered);

//         // if reset is true, reset d_argument_filtered_z. Else let d_argument_filtered_z be.
//         if(controller_reset)
//             d_argument_filtered_z = d_argument_filtered;
        
//         d_out = (d_argument_filtered - d_argument_filtered_z) / TIME_STEP;
//         d_argument_filtered_z = d_argument_filtered;

//         // calculate pre-saturated value
//         pre_sat_value = p_out + i_out + d_out;

//         // calculate post-saturated value
//         if (pre_sat_value > EXV_POSITION_MAX)
//             post_sat_value = EXV_POSITION_MAX;
//         else if(pre_sat_value < EXV_POSITION_MIN)
//             post_sat_value = EXV_POSITION_MIN;
//         else
//             post_sat_value = pre_sat_value;
        
//         // calculate back-calculation output for using in next time step
//         bc_out = BC_GAIN * (post_sat_value - pre_sat_value);
        
//         // end of pid code