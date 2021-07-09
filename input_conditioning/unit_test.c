#include <stdio.h>
#include "input_conditioning.h"

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
    
    float input;

    InputConditioning_Constructor(&input_conditioning_object, u_vector[0], kWindowSize, kTimeStep, kTau);

    printf("uinput %f window filter: %f lpf: %f\n", 
        input, 
        input_conditioning_object.window_filter_object.yk_,
        input_conditioning_object.low_pass_filter_object.yk_
        );

    for (int j = 1; j < sizeof(u_vector) / sizeof(u_vector[0]); j ++) {

        input = u_vector[j];

        if (j == 23) // test reset function
            InputConditioning_Reset(&input_conditioning_object, u_vector[j]);
        else {
            // run window filter
            InputConditioning_Step(&input_conditioning_object, input, false);
        }
        printf("j: %2d input: %f window filter: %f lpf: %f\n", 
        j,
        input, 
        input_conditioning_object.window_filter_object.yk_,
        input_conditioning_object.low_pass_filter_object.yk_
        );
        
    }

    return 0;
}