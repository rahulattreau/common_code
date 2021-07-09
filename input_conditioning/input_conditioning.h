/*
description:
1. This class serves to condition an input sensor.
2. Currently this involves a window filter and low pass filter
3. In future, this could be error checking for the sensor also
*/

#pragma once

#include "../window_filter/window_filter.h"
#include "../lpfo1/low_pass_filter_o1.h"

// define data type for input conditioning class
typedef struct {
    window_filter_t window_filter_object;
    low_pass_filter_o1_t low_pass_filter_object;
    float window_size;
    float time_step;
    float tau;
} input_conditioning_t;

void InputConditioning_Constructor(
    input_conditioning_t * const instance, 
    const float init_value,
    const float window_size,
    const float time_step,
    const float tau
    );

void InputConditioning_Reset(input_conditioning_t * const instance, const bool reset);

void InputConditioning_Step(input_conditioning_t * const instance, const float xk, const bool reset);