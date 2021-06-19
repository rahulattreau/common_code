#include "lookup_table.h"

float lookup_table(float *breakpoints, float *table_values, const int length, const float x_value) {

    float y_value = 0.0;

    // cycle through array to see if x_value lies between breakpoint entries
    for(int j = 0; j < (length - 1); j++) {
        
        // check if x_value is between consecutive breakpoints
        if ( ( x_value >= *(breakpoints + j) ) && ( x_value < *(breakpoints + j + 1) ) ) {
            
            /* Perform linear interpolation. Formula:
            y_value = y_table_value \
                + (delta between x_value and first breakpoint)
                    * (delta in consecutive y_table_values) / (delta in consecutive x_table_values)
            */
           
            y_value = *(table_values + j) \
                + ( x_value - *(breakpoints + j) ) * \
                ( *(table_values + j + 1) - *(table_values + j) ) \
                    / ( *(breakpoints + j + 1) - *(breakpoints + j) );
            
        }
    }

    return y_value;
}