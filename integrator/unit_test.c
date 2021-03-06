#include <stdio.h>
#include "control_common_code/integrator.h"

int main() {

    float time_step = 0.1;
    float u = 0.1;
    float init_val = 2.0;

    integrator_t y;
    IntegratorInit(&y, time_step);
    
    bool reset = false;
    
    for (float j = 0 + time_step; j < 10; j += time_step) {
        if (j > 1.0)
            u = 1.1;
        
        // test that reset gets activated
        if (j > 2.0)
            reset = true;
        
        // test that reset deactivates
        if (j > 3.5)
            u = 10.0;
        if (j > 4.1)
            reset = false;
        
        if (j > 5.0)
            u = 0.0;
        
        if (j > 3.0)
            init_val = 3.0;
        
        // execute low pass filter
        IntegratorStep(&y, u, init_val, reset);

        printf("time: %f reset: %d u: %f unit delay: %f y: %f\n", 
        j, 
        reset, 
        u, 
        y.accumulator.output_k_1.output, 
        y.output
        );
    }
    
    return 0;

}