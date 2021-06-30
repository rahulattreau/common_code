#include <stdbool.h>
#include "pid_controller.h"
#define NULL 0

typedef struct {
    float p_gain;
    float i_gain;
    float d_gain;
} pid_params_t;

void PidParamsAssigner(input_bus_t *const input_bus, const pid_params_t *const pid_params) {
    input_bus->p_gain = pid_params->p_gain;
    input_bus->i_gain = pid_params->i_gain;
    input_bus->d_gain = pid_params->d_gain;
}

int main() {

    input_bus_t *_superheat_control_input_bus;
    output_bus_t superheat_control_output_bus;

    const pid_params_t kSuperheatControlCoolingPidParams = {.p_gain = 2.0, .i_gain = 2.0, .d_gain = 2.0};
    const pid_params_t kSuperheatControlHeatingPidParams = {.p_gain = 3.0, .i_gain = 3.0, .d_gain = 3.0};
    
    float sensed_value = 1.0;
    
    PidParamsAssigner(_superheat_control_input_bus, &kSuperheatControlCoolingPidParams);

    PidControl_Constructor(&superheat_control_output_bus, _superheat_control_input_bus, &sensed_value);
    PidControl_Step(&superheat_control_output_bus, _superheat_control_input_bus);

    return 0;
}