#include <stdio.h>
#include "window_filter.h"

int main() {

    window_filter_t window_filter_values;
    float u_vector[] = {1.0, 1.1, 0.9, 1.2, 
    3.1, // big deviation but still within range
    1.2, 1.3, 
    3.5, // out of range
    1.1, 0.9, 1.0, 1.1, 0.9, 1.2, 
    3.1,  // big deviation but still within range
    1.2, 
    3.3, 3.5, // out of range
    1.1, 0.9,
    1.2, 
    3.3, 3.5, // test reset
    1.1, 0.9,
    };
    
    float input;

    WindowFilter_Constructor(&window_filter_values, -5.0, 2.0);

    for (int j = 0; j < sizeof(u_vector) / sizeof(u_vector[0]); j ++) {

        // initialize value
        if (j == 0)
            WindowFilter_Reset(&window_filter_values, u_vector[0]);

        input = u_vector[j];

        if (j == 23) // test reset function
            WindowFilter_Reset(&window_filter_values, u_vector[j]);

        // run window filter
        WindowFilter_Step(&window_filter_values, &input, false);
        printf("u: %f input: %f\n", input, window_filter_values.value);
        
    }

    return 0;
}