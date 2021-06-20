/*
Description:
1. If absolute difference in value from current time step to previous time step 
is greater than window size, retain value.
2. Else, use new value
*/

// type definition
typedef struct {
    float value; // state variable
    float value_z; // state variable from previous time step
} window_filter_t;

// declare initialization function
void WindowFilterInit(window_filter_t * const instance, const float init_value);

// declare window filter function
void WindowFilterFunc(window_filter_t * const instance, const float input_value, const float window_size);

// declare store state variable
void WindowFilterStoreStateVariable(window_filter_t * const instance);