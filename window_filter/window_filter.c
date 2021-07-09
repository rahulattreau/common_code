#include "window_filter.h"

// define constructor
void WindowFilter_Constructor(window_filter_t * const instance, const float init_value, const float window_size) {
    instance->window_size_ = window_size;

    WindowFilter_Reset(instance, init_value);
}

void WindowFilter_Reset(window_filter_t * const instance, const float xk) {
    instance->yk_ = xk;
    instance->yk_1_ = xk;
}

// define window filter function
void WindowFilter_Step(window_filter_t * const instance, const float xk, const bool reset) {
    
    const float delta_in_consecutive_states = xk - instance->yk_;

    // if reset is true, then call reset function, else perform window filtering
    if (reset)
        WindowFilter_Reset(instance, xk);
    
    else {
        // if xk is inside range, store xk into window filter state variable
        if(
            ( delta_in_consecutive_states <= instance->window_size_ ) &&
            ( delta_in_consecutive_states >= -instance->window_size_ )
            )
            instance->yk_ = xk;
        
        else
            instance->yk_ = instance->yk_1_;
        
        // store xk for next time step
        instance->yk_1_ = xk;
    }
}