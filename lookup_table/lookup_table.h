/* Description:
1. Look-up table with linear inerpolation
2. Extrapolates if values outside of range
*/

// define lookup table datatype
typedef struct {
    float *breakpoints_data;
    float *table_values_data;
    int length;
} lookup_table_data_t;

// declare lookuptable data type initialization function
lookup_table_data_t LookupTableTypeInit(float *breakpoints_data, float *table_values_data);

// declare lookup_table function prototype
// float LookupTableFunc(float const *breakpoints, float const *table_values, const int length, const float x_value);
float LookupTableFunc(lookup_table_data_t *lookup_table_data, const float x_value);