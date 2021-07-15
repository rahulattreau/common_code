#include <stdio.h>
#include <stdbool.h>
#include "unit_delay.h"

int main() {

    float u_vector[] = {1.0, 1.1, 0.9, 1.2, 
    3.1, 
    1.2, 1.3, 
    3.5, 
    1.1, 0.9, 1.0, 1.1, 0.9, 1.2, 
    3.1, 
    1.2, 
    3.3, 3.5, 
    1.1, 0.9,
    1.2, 
    3.3, 3.5, 
    1.1, 0.9,
    };

    int vector_length = sizeof(u_vector) / sizeof(u_vector[0]);
    bool reset_vector[vector_length]; 
    for (int j = 0; j<=vector_length; j++ ) {

        reset_vector[j] = false;

    }

    reset_vector[4] = true;
    reset_vector[5] = true;
    reset_vector[13] = true;
    
    unit_delay_t unit_delay;
    UnitDelay_Constructor(&unit_delay);
    // UnitDelay_Init(&unit_delay, u_vector[0]);

    float u;
    bool reset = false;
    bool init = false;

    for(int j = 0; j<25; j++) {

        u = u_vector[j];
        reset = reset_vector[j];

        UnitDelay_Step(&unit_delay, u, reset);
        
        printf("j: %2d reset: %d u: %f y: %f \n", j, reset, u, unit_delay.yk_);

        UnitDelay_PostStep(&unit_delay, u);
    }

    return 0;
}