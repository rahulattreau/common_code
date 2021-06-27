#include <stdbool.h>
#include "pid_controller.h"

int main() {

    input_bus_t superheat_control_input_bus;
    float * _reference;
    float * _sensed_value;
    bool * _reset;

    PidControl_Constructor(&superheat_control_input_bus, _reference, _sensed_value, _reset);

    return 0;
}