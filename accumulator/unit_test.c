#include <stdio.h>
#include "control_common_code/accumulator.h"

int main() {

    float time_step = 0.1;
    float u = 0.1;
    float init_val = 3.0;

    accumulator_t y;
    AccumulatorInit(&y);
    
    bool reset = false;
    
    for (float j = 0; j < 10; j += time_step) {
        if (j > 1.0)
            u = 1.1;
        
        // test that reset gets activated
        if (j > 2.0)
            reset = true;
        
        // test that reset deactivates
        if (j > 3.5) {
            init_val = 10.0;
            u = 1.0;
        }
        if (j > 4.1)
            reset = false;
        
        if (j > 5.0)
            u = 0.0;
        
        // execute low pass filter
        AccumulatorStep(&y, u, init_val, reset);

        printf("time: %f reset: %d init_val %f u: %f unit delay: %f y: %f\n", j, reset, init_val, u, y.output_k_1.output, y.output);
    }
    
    return 0;

}