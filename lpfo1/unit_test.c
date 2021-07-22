#include<stdio.h>
#include "control_common_code/low_pass_filter_order_1.h"

int main() {

    float time_step = 0.01;
    float u = 0.1;

    low_pass_filter_o1_t y;
    LowPassFilterO1_Constructor(&y, time_step, 1.0);

    bool reset = false;
    bool init = false;

    for (float j = 0; j < 10; j += time_step) {

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

        printf("time: %f reset %d u: %f unit delay: %f y: %f\n", j, reset, u, y.output_k_1.output, y.output);
    }
    
    return 0;

}