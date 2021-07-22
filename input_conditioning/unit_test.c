#include <stdio.h>
#include "control_common_code/input_conditioning.h"

int main() {

    const float kWindowSize = 2.0;
    const float kTimeStep = 0.1;
    const float kTau = 0.4;

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
    
    input_conditioning_t input_conditioning_object;
    
    float input = u_vector[0];
    const bool kWindowFilterActive = true;

    InputConditioningInit(&input_conditioning_object, kWindowSize, kTimeStep, kTau, kWindowFilterActive);
    
    for (int j = 0; j < sizeof(u_vector) / sizeof(u_vector[0]); j ++) {

        input = u_vector[j];

        InputConditioningStep(&input_conditioning_object, input);
        
        printf("j: %2d input: %f window filter: %f lpf: %f output: %f\n", 
        j,
        input, 
        input_conditioning_object.window_filter_object.output,
        input_conditioning_object.low_pass_filter_object.output,
        input_conditioning_object.output
        );
        
    }

    return 0;
}