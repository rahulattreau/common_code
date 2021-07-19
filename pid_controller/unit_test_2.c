#include <stdio.h>
#include "pid_controller.h"
#include "../window_filter/window_filter.h"
#include "../lpfo1/low_pass_filter_o1.h"

#define EXV_POSITION_MAX              (500)
#define EXV_POSITION_MIN              (12)
#define EXV_HOME_POSITION           (200)
#define EXV_HOME_OVERDRIVE_STEPS     (60)
#define EXV_CLOSE_OVERDRIVE_STEPS     (8)
#define EXV_INIT_POS (32)

#define TIME_STEP      (0.100)   // seconds
#define TIME_STEP_MS      (100)   // ms 

// this is a struct definition for pid params that change in cooling/heating
typedef struct {
    float p_gain;
    float i_gain;
    float d_gain;
    float deadzone_up;
    float deadzone_lo;
} pid_params_t;

// saturation temperature calculation declaration
float CalcSatTemp(float pressure);

// declaration of function for assinging pid parameters specific to cooling or heating
void PidParamsAssigner(input_bus_t *const input_bus, const pid_params_t *const pid_params);

// declaration for superheat control parameter scheduling
void SuperheatControlParamterScheduling(
    input_bus_t *const input_bus, 
    const pid_params_t *const pid_params_heating, 
    const pid_params_t *const pid_params_cooling
    );


// declaration for superheat control parameter scheduling
void SuperheatControlParamterScheduling(
    input_bus_t *const input_bus, 
    const pid_params_t *const pid_params_heating, 
    const pid_params_t *const pid_params_cooling
    );


int main()
{
    
    const float kSuperheatWindowFilterBand = 2.0;
    const float kSuperheatWindowFilterInitValue = 0.0;
    const float kSuperheatLowPassFilterTau = 1.0;
    const float kSuperheatLowPassFilterInitValue = 0.0;
    float superheat = 1.1;
    static bool superheat_valid = false;
    static float time_value = 0.0;
        
    static window_filter_t superheat_window_values;
    // note: superheat does not exist yet so the init value is a dummy value. The long term solution is that superheat must be caclulated outside of exv control
    WindowFilter_Constructor(&superheat_window_values, kSuperheatWindowFilterInitValue, kSuperheatWindowFilterBand);

    static low_pass_filter_o1_t superheat_filtered;
    // note: superheat does not exist yet so the init value is a dummy value. The long term solution is that superheat must be caclulated outside of exv control
    LowPassFilterO1_Constructor(&superheat_filtered, kSuperheatLowPassFilterInitValue, TIME_STEP, kSuperheatLowPassFilterTau);

    // define pid data, assign values, and call constructor

    const float kSuperheatControlReference = 8.0;

    // define constants specific to cooling mode pid
    const pid_params_t kSuperheatControlCoolingPidParams = {
        .p_gain = -7.966168371361135, 
        .i_gain = -0.14572259215904518, 
        .d_gain = 0.0, 
        .deadzone_up = 1.0, 
        .deadzone_lo = -2.0
        };
    // define constants for specific to heating mode pid
    const pid_params_t kSuperheatControlHeatingPidParams = {
        .p_gain = -0.9961465361751612, 
        .i_gain = -0.014942198042627418, 
        .d_gain = 0.0, 
        .deadzone_up = 1.0, 
        .deadzone_lo = -3.0
        };
    // create pid control input and output objects and assign variables that are independent of cooling/heating:
    /*  note: the input struct is large and has:
        1. signals that are independent of cooling and heating - these have been assigned on definition
        2. signals that change based on whether the system is in cooling and heating - using cooling values as a default for now
        3. pointer to sensor - this is set in the constructor 
    */
    input_bus_t superheat_control_input_bus = {
        .reset = true,
        .bc_gain = 1.0,
        .d_filter_tau = 0.0,
        .up_sat_value = EXV_POSITION_MAX,
        .lo_sat_value = EXV_POSITION_MIN,
        .init_value = EXV_INIT_POS,
        .time_step = TIME_STEP
        };
    pid_control_bus_t superheat_control_output_bus = {
        .i_out_bus.i_out = superheat_control_input_bus.init_value,
        .sat_and_sum_out_bus.pre_sat_value = superheat_control_input_bus.init_value
    };
    // assign variables that are dependent on cooling/heating
    PidParamsAssigner(&superheat_control_input_bus, &kSuperheatControlCoolingPidParams);
    // superheat_control_input_bus.p_gain = kSuperheatControlCoolingPidParams.p_gain;
    // superheat_control_input_bus.i_gain = kSuperheatControlCoolingPidParams.i_gain;
    // superheat_control_input_bus.d_gain = kSuperheatControlCoolingPidParams.d_gain;
    // superheat_control_input_bus.dead_zone_up = kSuperheatControlCoolingPidParams.deadzone_up;
    // superheat_control_input_bus.dead_zone_lo = kSuperheatControlCoolingPidParams.deadzone_lo;

    // call constructor
    PidControl_Constructor( &superheat_control_output_bus, &superheat_control_input_bus, &(superheat_filtered.yk_) );

    superheat_valid = false;

    for (int j = 0; j<20; j++) {
        
        // superheat window filter
        WindowFilter_Step(&superheat_window_values, &superheat);

        // superheat low pass filter
        LowPassFilterO1_Step(&superheat_filtered, superheat_window_values.value, !superheat_valid);

        // schedule parameters based on heating/cooling void SuperheatControlModeGainScheduler();
        SuperheatControlParamterScheduling(
            &superheat_control_input_bus, 
            &kSuperheatControlHeatingPidParams, 
            &kSuperheatControlCoolingPidParams
            );
        

        // calculate reset. reset is true when controller needs to be off.
        superheat_control_input_bus.reset = !(superheat_valid);

        // set reference
        superheat_control_input_bus.reference = kSuperheatControlReference;

        // run pid step function
        PidControl_Step(&superheat_control_output_bus, &superheat_control_input_bus);

        // print data
        printf("u = %f, y = %f \n", *(superheat_control_input_bus.sensed_value), superheat_control_output_bus.sat_and_sum_out_bus.post_sat_value);

    }

    return 0;
}

// define function for assigning pid parameters
void PidParamsAssigner(input_bus_t *const input_bus, const pid_params_t *const pid_params) {
    input_bus->p_gain = pid_params->p_gain;
    input_bus->i_gain = pid_params->i_gain;
    input_bus->d_gain = pid_params->d_gain;
    input_bus->dead_zone_up = pid_params->deadzone_up;
    input_bus->dead_zone_lo = pid_params->deadzone_lo;
}

// definition of superheat control parameter scheduling
void SuperheatControlParamterScheduling(
    input_bus_t *const _input_bus, 
    const pid_params_t *const _pid_params_heating, 
    const pid_params_t *const _pid_params_cooling
    ) {
        PidParamsAssigner(_input_bus, _pid_params_heating);
    // if ( get_reverser_state( get_current_level() ) == HEATING )
    //         PidParamsAssigner(_input_bus, _pid_params_heating);
    //     else
    //         PidParamsAssigner(_input_bus, _pid_params_cooling);
}