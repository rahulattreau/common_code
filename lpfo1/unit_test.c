#include<stdio.h>
#include "low_pass_filter_o1.h"

int main() {

    float time_step = 0.1;
    float u = 0.5;

    low_pass_filter_o1_t y;
    LowPassFilterO1_Constructor(&y, u, time_step, 1.0);

    bool reset = false;

    for (float j = 0; j < 20; j += time_step) {
        if (j > 1.0)
            u = 1.0;
        
        // test that reset gets activated
        if (j > 2.0)
            reset = true;
        
        // test that reset deactivates
        if (j > 4.0)
            reset = false;
        
        if (j > 5.0)
            u = 0.0;
        
        LowPassFilterO1_Step(&y, u, reset);

        printf("time: %f u: %f y: %f\n", j, u, y.yk_);
    }
    

    return 0;

}