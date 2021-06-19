#include "linear_interpolation.h"

// define function
float LinearInterpolation(const float x1, const float x2, const float y1, const float y2, const float x) {
    
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
    float y = y1 + (x-x1)/(x2-x1)*(y2-y1);
    
    return y;

}