#include <stdio.h>
#include "control_common_code/window_filter.h"

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

    float y_vector_correct[] = {
        1.0, 1.1, 0.9, 1.2, 
        3.1, // big deviation but still within range
        1.2, 1.3, 
        1.3, // 3.5, // out of range
        1.1, 0.9, 1.0, 1.1, 0.9, 1.2, 
        3.1,  // big deviation but still within range
        1.2, 
        1.2, // holds value 
        3.3, 3.5, // out of range
        1.1, 
        // 0.9,
        1.2, 
        1.2, // hold value 
        3.3, 
        // 3.5, // test reset
        1.1, 0.9,
    };
    
    float input;
    bool reset = false;

    WindowFilter_Constructor(&window_filter_values, 2.0);

    for (int j = 0; j < sizeof(u_vector) / sizeof(u_vector[0]); j ++) {

        input = u_vector[j];

        if (j == 23) // test reset function
            reset = true;
        else {
            reset = false;
        }

        WindowFilter_Step(&window_filter_values, input, reset);
        
        printf("reset: %d u: %f output: %f error: %f\n", reset, input, window_filter_values.output, 
        y_vector_correct[j] - window_filter_values.output);
        
    }

    return 0;
    
}