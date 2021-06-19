#include <stdbool.h>
#include "lookup_table.h"
#include "linear_interpolation.h"

// define lookup_table function
float LookupTableFunc(float const *breakpoints, float const *table_values, const int length, const float x_value) {

    float y_value = 0.0;
    bool outside_range = true;

    // cycle through array to see if x_value lies between breakpoint entries
    for(int j = 0; j < (length - 1); j++) {
        
        // check if x_value is between consecutive breakpoints
        if ( ( x_value >= *(breakpoints + j) ) && ( x_value <= *(breakpoints + j + 1) ) ) {
            
            /* Perform linear interpolation
            Formula:
            y = y1 + (x-x1)/(x2-x1)*(y2-y1)
                where:
                y - y_value
                y1 - first table_value in range
                y2 - second table_value in range
                x1 - first breakpoint value in range
                x2 - second breakpoint value in range
                x - input x value for which y_value is desired
            */
            y_value = *(table_values + j) \
            + ( x_value - *(breakpoints + j) ) \
            * ( *(table_values + j + 1) - *(table_values + j) ) \
            / ( *(breakpoints + j + 1) - *(breakpoints + j) );
            
            outside_range = false;
        }

        if (outside_range) {
            if(x_value < breakpoints[0]) { 
                int j = 0;
                y_value = *(table_values + j) \
                + ( x_value - *(breakpoints + j) ) \
                * ( *(table_values + j + 1) - *(table_values + j) ) \
                / ( *(breakpoints + j + 1) - *(breakpoints + j) );
            }
            else if(x_value > breakpoints[length - 1]) {
                int j = length - 2;
                y_value = *(table_values + j) \
                + ( x_value - *(breakpoints + j) ) \
                * ( *(table_values + j + 1) - *(table_values + j) ) \
                / ( *(breakpoints + j + 1) - *(breakpoints + j) );
            }
        }
    }

    return y_value;
}