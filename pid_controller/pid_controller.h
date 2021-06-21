// define integral output data type:
typedef struct {
    float integrand;
    float signum_integrand;
    float pre_sat_dead_zone;
    float signum_dead_zone_out;
    float i_out;
} i_out_bus_t;

// define differential output data type
typedef struct {
    float d_argument;
    float d_out;
    float d_argument_filtered;
    float d_argument_filtered_z;
} d_out_bus_t;

// define sum and sat output data type:
typedef struct {
    float pre_sat_value;
    float post_sat_value;
    float bc_out;
} sat_and_sum_bus_t;

// ===== controller level data types =====

// define parameter struct data type
typedef struct {
    float p_gain;
    float i_gain;
    float d_gain;
    float bc_gain;
    float d_filter_tau;
    float up_sat_value;
    float lo_sat_value;
    float init_value;
    float time_step;
    
} parameters_t;

// define input struct data type
typedef struct {
    float *reference;
    float *sensed_value;
    float *reset;
} input_bus_t;

// outputs
typedef struct {
    float error;
    float p_out_bus;
    i_out_bus_t integral_function_out_bus;
    d_out_bus_t d_out_bus;
    sat_and_sum_bus_t sat_and_sum_out_bus;
} output_bus_t;

// ===== define member functions =====

void PidControl_Constructor(
    input_bus_t *_input_bus,
    float *reference_pointer,
    float *sensed_value_pointer,
    bool *reset_pointer
    );