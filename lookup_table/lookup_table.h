#pragma once

// define macro for getting the length of an array
#define ArrayLength(array) ( sizeof(array)/sizeof(array[0]) )

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
void LookupTable_Constructor(
    lookup_table_data_t * const instance, 
    float *breakpoints_data, 
    float *table_values_data,
    int length
    );

// declare lookup_table function prototype
float LookupTable(
    lookup_table_data_t * const instance, 
    const float x_value
    );