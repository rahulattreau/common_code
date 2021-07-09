#include<stdio.h>
#include "low_pass_filter_o1.h"

int main() {

    float time_step = 0.01;
    float u = 0.1;

    low_pass_filter_o1_t y;
    LowPassFilterO1_Constructor(&y, u, time_step, 1.0);
    LowPassFilterO1_Init(&y, 0.1);

    bool reset = false;

    // printf("alpha value = %f \n", y.alpha_);

    // initialize lpf
    
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
        LowPassFilterO1_Step(&y, u, reset);

        printf("time: %f u: %f unit delay: %f y: %f\n", j, u, y.yk_1_.yk_, y.yk_);
    }
    

    return 0;

}