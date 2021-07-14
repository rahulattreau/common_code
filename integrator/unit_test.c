#include<stdio.h>
#include "integrator.h"

int main() {

    float time_step = 0.01;
    float u = 0.1;

    integrator_t y;
    Integrator_Constructor(&y, time_step);
    // initialize lpf
    Integrator_Init(&y, u);

    bool reset = false;
    
    printf("time: %f u: %f unit delay: %f y: %f\n", 0.0, u, y.yk_1_.yk_, y.yk_);

    for (float j = 0 + time_step; j < 10; j += time_step) {
        if (j > 1.0)
            u = 1.1;
        
        // test that reset gets activated
        if (j > 2.0)
            reset = true;
        
        // test that reset deactivates
        if (j > 4.0)
            u = 10.0;
        if (j > 4.1)
            reset = false;
        
        if (j > 5.0)
            u = 0.0;
        
        // execute low pass filter
        Integrator_Step(&y, u, reset);

        printf("time: %f reset: %d u: %f unit delay: %f y: %f\n", j, reset, u, y.yk_1_.yk_, y.yk_);
    }
    
    return 0;

}