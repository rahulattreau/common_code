/*
description:
1. This class serves to condition an input sensor.
2. Currently this involves a window filter and low pass filter
3. In future, sensor error checking could be added too
*/

#ifndef INPUT_CONDITIONING_H
#define INPUT_CONDITIONING_H

#include "../../window_filter/control_common_code/window_filter.h"
#include "../../lpfo1/control_common_code/low_pass_filter_order_1.h"

// define data type for input conditioning class
typedef struct {
    window_filter_t window_filter_object;
    low_pass_filter_o1_t low_pass_filter_object;
    bool window_filter_enable; // used to enable window filter
    float output; // output
} input_conditioning_t;

void InputConditioning_Constructor(
    input_conditioning_t * const instance, 
    const float window_size,
    const float time_step,
    const float tau, 
    const bool window_filter_active
    );

void InputConditioning_Step(input_conditioning_t * const instance, const float u);

#endif // INPUT_CONDITIONING_H