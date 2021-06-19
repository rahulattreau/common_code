#include <stdbool.h>
#include "lookup_table.h"
#include "linear_interpolation.h"

// define lookup_table function
float LookupTableFunc(float const *breakpoints, float const *table_values, const int length, const float x_value) {

    float y_value = 0.0;
    
    // outside_range stores that the value was outside the x-range of the lookup-table data so that 
    // extrapolation can be performed if required
    bool outside_range = true;
    int first_breakpoint_index = 0;

    /*
    Procedure:
    1. Find the first breakpoint index that needs to be used for interpolation
    2. After that perform linear interpolation
    */

    // cycle through array to see if x_value lies between breakpoint entries
    for(int j = 0; j < (length - 1); j++) {
        
        // check if x_value is between consecutive breakpoints
        if ( ( *(breakpoints + j) <= x_value  ) && ( x_value <= *(breakpoints + j + 1) ) ) {
            
            first_breakpoint_index = j;
            
            // record that the value has been found in range
            outside_range = false;
        }
    }

    // if the value is not in the minimum and maximum breakpoint range:
    if (outside_range) {

        // if value is below the minimum breakpoint value:
        if(x_value < breakpoints[0])
            first_breakpoint_index = 0;

        // if value is above the maximum breakpoint value:
        else // if(x_value > breakpoints[length - 1])
            first_breakpoint_index = length - 2;

    }

    // calculate the second breakpoint
    const int kSecondBreakpointIndex = first_breakpoint_index + 1;
            
    // linearly extrapolate below range
    y_value = LinearInterpolation(breakpoints[first_breakpoint_index], // x1
        breakpoints[kSecondBreakpointIndex], // x2
        table_values[first_breakpoint_index], // y1
        table_values[kSecondBreakpointIndex], // y2
        x_value // x
        );

    return y_value;

}