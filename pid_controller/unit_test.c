#include <stdbool.h>
#include "pid_controller.h"

int main() {

    input_bus_t superheat_control_input_bus;
    output_bus_t superheat_control_output_bus;
    
    float reference = 5.0;
    float sensed_value = 1.0;
    bool reset = false;
    
    float *_reference = &reference;
    float *_sensed_value = &sensed_value;
    bool *_reset = &reset;

    PidControl_Constructor(&superheat_control_output_bus, &superheat_control_input_bus, _reference, _sensed_value, _reset);

    return 0;
}