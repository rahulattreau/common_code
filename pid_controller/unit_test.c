#include <stdbool.h>
#include "pid_controller.h"

int main() {

    input_bus_t superheat_control_input_bus;
    output_bus_t superheat_control_output_bus;
    
    const pid_params_t kPidCoolingParams = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};
    const pid_params_t kPidHeatingParams = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};
    pid_params_t pid_params;

    float reference = 5.0;
    float sensed_value = 1.0;
    bool reset = false;
    
    float *_reference = &reference;
    float *_sensed_value = &sensed_value;
    bool *_reset = &reset;

    pid_params = kPidCoolingParams;
    PidControl_Constructor(&superheat_control_output_bus, &superheat_control_input_bus, _reference, _sensed_value, _reset, &pid_params);
    PidControl_Step(&superheat_control_input_bus, &superheat_control_output_bus, &pid_params);

    return 0;
}