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
    
    unit_delay_t unit_delay;
    UnitDelay_Constructor(&unit_delay);
    // UnitDelay_Init(&unit_delay, u_vector[0]);

    float u;
    bool reset = false;
    bool init = false;

    for(int j = 0; j<25; j++) {
        u = u_vector[j];

        // if (j == 0)
        //     reset = true;
        // else if (j == 4 || j == 5)
        //     reset = true;
        // else
        //     reset = false;
        if (j == 4 || j == 5)
            reset = true;
        else
            reset = false;
        UnitDelay_Step(&unit_delay, u, reset);
        
        printf("j: %2d reset: %d u: %f y: %f \n", j, reset, u, unit_delay.yk_);

        UnitDelay_PostStep(&unit_delay, u);
    }

    return 0;
}