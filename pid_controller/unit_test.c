#include <stdio.h>
#include "control_common_code/pid_controller.h"
// #define NULL 0

// this definition needs to move to the firmware
typedef struct {
    float p_gain;
    float i_gain;
    float d_gain;
    float deadzone_up;
    float deadzone_lo;
} pid_params_t;

// this needs to move to the firmware
void PidParamsAssigner(input_bus_t *const input_bus, const pid_params_t *const pid_params) {
    input_bus->p_gain = pid_params->p_gain;
    input_bus->i_gain = pid_params->i_gain;
    input_bus->d_gain = pid_params->d_gain;
    input_bus->dead_zone_up = pid_params->deadzone_up;
    input_bus->dead_zone_lo = pid_params->deadzone_lo;

}

int main() {

    const bool kInitReset = true;
    float reference = 8.0;
    float init_value = 20.0;

    input_bus_t superheat_control_input_bus = {
        .reference = 8.0,
        .reset = true,
        .bc_gain = 1.0,
        .d_filter_tau = 1.0,
        .up_sat_value = 100.0,
        .lo_sat_value = 20.0,
        .init_value = 20.0,
        .time_step = 0.1
        };

    output_bus_t superheat_control_output_bus = {
        // .i_out_bus.i_out = 20.0,
        .sat_and_sum_out_bus.pre_sat_value = 0.0
    };
    // output_bus_t superheat_control_output_bus;

    // this needs to move to the firmware
    const pid_params_t kSuperheatControlCoolingPidParams = {
        .p_gain = 2.0, 
        .i_gain = 2.0, 
        .d_gain = 2.0, 
        .deadzone_up = 1.0, 
        .deadzone_lo = -2.0
        };
    // this needs to move to the firmware
    const pid_params_t kSuperheatControlHeatingPidParams = {
        .p_gain = 3.0, 
        .i_gain = 3.0, 
        .d_gain = 3.0, 
        .deadzone_up = 1.0, 
        .deadzone_lo = -3.0
        };
    
    float sensed_value = 6.0;
    
    PidParamsAssigner(&superheat_control_input_bus, &kSuperheatControlCoolingPidParams);

    PidControl_Constructor(&superheat_control_output_bus, &superheat_control_input_bus, &sensed_value);
    
    printf("d_out filter alpha = %f\n", 
        superheat_control_output_bus.d_out_bus.d_lpf.alpha_
        );
    
    // printf("tick number = %d \n y = %f \n p_out = %f\n i_out = %f\n d_out = %f\n", 
    //     0,
    //     superheat_control_output_bus.sat_and_sum_out_bus.post_sat_value,
    //     superheat_control_output_bus.p_out,
    //     superheat_control_output_bus.i_out_bus.i_out,
    //     superheat_control_output_bus.d_out_bus.d_argument_filtered
    //     );

    for (int j = 0; j<20; j++) {

        // set values
        if (j > 5 && j < 15)
            superheat_control_input_bus.reset = false;
        else
            superheat_control_input_bus.reset = true;
        
        if (j > 1 && j < 3)
            superheat_control_input_bus.init_value = 50.0;
        else
            superheat_control_input_bus.init_value = 20.0;

        if (j > 10 && j < 13)
            superheat_control_input_bus.init_value = 60.0;
        else
            superheat_control_input_bus.init_value = 20.0;

        if (j > 10 && j < 15)
            PidParamsAssigner(&superheat_control_input_bus, &kSuperheatControlHeatingPidParams);
        else
            PidParamsAssigner(&superheat_control_input_bus, &kSuperheatControlCoolingPidParams);
        
        PidControl_Step(&superheat_control_output_bus, &superheat_control_input_bus);
        
        // printf("tick number = %d \n", j);
        printf("tick number = %d \n y = %f \n p_out = %f\n i_out = %f\n d_out = %f\n", 
        j,
        superheat_control_output_bus.sat_and_sum_out_bus.post_sat_value,
        superheat_control_output_bus.p_out,
        superheat_control_output_bus.i_out_bus.integrator.yk_,
        superheat_control_output_bus.d_out_bus.d_out
        );

    }

    return 0;
}