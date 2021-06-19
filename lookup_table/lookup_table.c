#include <stdbool.h>
#include "lookup_table.h"
#include "linear_interpolation.h"

// define lookup_table function
float LookupTableFunc(float const *breakpoints, float const *table_values, const int length, const float x_value) {

    float y_value = 0.0;
    
    // outside_range stores that the value was outside the x-range of the lookup-table data so that 
    // extrapolation can be performed if required
    bool outside_range = true;

    // cycle through array to see if x_value lies between breakpoint entries
    for(int j = 0; j < (length - 1); j++) {
        
        // check if x_value is between consecutive breakpoints
        if ( ( *(breakpoints + j) <= x_value  ) && ( x_value <= *(breakpoints + j + 1) ) ) {
            
            y_value = LinearInterpolation(*(breakpoints + j), // x1
                *(breakpoints + j + 1), // x2
                *(table_values + j), // y1
                *(table_values + j + 1), // y2
                x_value // x
                );

            // found that the value is in range
            outside_range = false;
        }

        // if the value is not in the minimum and maximum breakpoint range:
        if (outside_range) {

            // if value is below the minimum breakpoint value:
            if(x_value < breakpoints[0]) { 
                
                // define breakpoint indexes
                const int kFirstBreakpointIndex = 0;
                const int kSecondBreakpointIndex = 1;
                
                // linearly extrapolate below range
                y_value = LinearInterpolation(breakpoints[kFirstBreakpointIndex], // x1
                    breakpoints[kSecondBreakpointIndex], // x2
                    table_values[kFirstBreakpointIndex], // y1
                    table_values[kSecondBreakpointIndex], // y2
                    x_value // x
                    );

            }

            // if value is above the maximum breakpoint value:
            else // if(x_value > breakpoints[length - 1])
            {
                // define breakpoint indexes
                const int kFirstBreakpointIndex = length - 2;
                const int kSecondBreakpointIndex = kFirstBreakpointIndex + 1;
                
                // linearly extrapolate below range
                y_value = LinearInterpolation(breakpoints[kFirstBreakpointIndex], // x1
                    breakpoints[kSecondBreakpointIndex], // x2
                    table_values[kFirstBreakpointIndex], // y1
                    table_values[kSecondBreakpointIndex], // y2
                    x_value // x
                    );
            }
        }
    }

    return y_value;
}