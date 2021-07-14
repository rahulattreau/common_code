#include "window_filter.h"

// define constructor
void WindowFilter_Constructor(window_filter_t * const instance, const float window_size) {

    instance->window_size_ = window_size;
    UnitDelay_Constructor( &(instance->yk_1_) );

}

// define window filter function
void WindowFilter_Step(window_filter_t * const instance, const float u, const bool reset) {
    
    const float delta_in_consecutive_states = u - instance->yk_;

    // run unit delay
    UnitDelay_Step( &(instance->yk_1_), u, reset );

    // if u is inside range, set output to u, else set output to unit delayed value
    if(
        ( delta_in_consecutive_states <= instance->window_size_ ) &&
        ( delta_in_consecutive_states >= -instance->window_size_ )
        )
        instance->yk_ = u;
    
    else
        instance->yk_ = instance->yk_1_.yk_;
    
    // run post step functions
    UnitDelay_PostStep( &(instance->yk_1_), u );
    
}