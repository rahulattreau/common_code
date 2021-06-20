#include "window_filter.h"

// initialization function
void WindowFilterInit(window_filter_t * const instance, const float init_value) {
    instance->value = init_value;
    instance->value_z = init_value;
}

// window filter function
void WindowFilterFunc(window_filter_t * const instance, const float input_value, const float window_size) {
    if (
        ( (input_value - instance->value) > window_size ) ||
        ( (input_value - instance->value) < -window_size )
        )

        instance->value = instance->value_z;
    else
        instance->value = input_value;
    
    instance->value_z = instance->value;
}