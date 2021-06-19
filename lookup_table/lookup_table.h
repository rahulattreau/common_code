/* Description:
1. Look-up table with linear inerpolation
2. Extrapolates if values outside of range
*/

// define lookup table struct datatype
typedef struct {
    float *breakpoints_data;
    float *table_values_data;
    int length;
} lookup_table_data_t;

// declare lookuptable data type initialization function
lookup_table_data_t LookupTable__Constructor(
    lookup_table_data_t * const me, 
    float *breakpoints_data, 
    float *table_values_data
    );

// declare lookup_table function prototype
float LookupTable__Func(lookup_table_data_t * const me, const float x_value);