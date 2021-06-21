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
void LookupTableInit(
    lookup_table_data_t * const instance, 
    float *breakpoints_data, 
    float *table_values_data
    );

// declare lookup_table function prototype
float LookupTable(
    lookup_table_data_t * const instance, 
    const float x_value
    );