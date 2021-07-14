/*
description:
1. This class serves to condition an input sensor.
2. Currently this involves a window filter and low pass filter
3. In future, sensor error checking could be added too
*/

#pragma once

#include "../window_filter/window_filter.h"
#include "../lpfo1/low_pass_filter_o1.h"

// define data type for input conditioning class
typedef struct {
    window_filter_t window_filter_object_;
    low_pass_filter_o1_t low_pass_filter_object_;
    bool window_filter_active_;
    float yk_; // output
} input_conditioning_t;

void InputConditioning_Constructor(
    input_conditioning_t * const instance, 
    const float window_size,
    const float time_step,
    const float tau, 
    const bool window_filter_active
    );

void InputConditioning_Init(input_conditioning_t * const instance, const float xk);

void InputConditioning_Step(input_conditioning_t * const instance, const float xk);