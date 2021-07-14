#include<stdio.h>
#include "accumulator.h"

int main() {

    float time_step = 0.1;
    float u = 0.1;
    float init_val = 2.0;

    integrator_t y;
    Accumulator_Constructor(&y, time_step);
    // initialize lpf
    Accumulator_Init(&y, init_val);

    bool reset = false;
    
    printf("time: %f reset: %d init_val %f u: %f unit delay: %f y: %f\n", 0.0, reset, init_val, u, y.yk_1_.yk_, y.yk_);

    for (float j = 0 + time_step; j < 10; j += time_step) {
        if (j > 1.0)
            u = 1.1;
        
        // test that reset gets activated
        if (j > 2.0)
            reset = true;
        
        // test that reset deactivates
        if (j > 4.0) {
            init_val = 10.0;
            u = 1.0;
        }
        if (j > 4.1)
            reset = false;
        
        if (j > 5.0)
            u = 0.0;
        
        // execute low pass filter
        Accumulator_Step(&y, u, init_val, reset);

        printf("time: %f reset: %d init_val %f u: %f unit delay: %f y: %f\n", j, reset, init_val, u, y.yk_1_.yk_, y.yk_);
    }
    
    return 0;

}