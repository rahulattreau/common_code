#include <stdio.h>
#include "window_filter.h"

int main() {

    float_state_and_state_z u;
    float u_vector[] = {1.0, 1.1, 0.9, 1.2, 
    3.1, // big deviation but still within range
    1.2, 1.3, 
    3.5, // out of range
    1.1, 0.9, 1.0, 1.1, 0.9, 1.2, 
    3.1,  // big deviation but still within range
    1.2, 
    3.3, 3.5, // out of range
    1.1, 0.9};
    
    u.value = u_vector[0];
    u.value_z = u_vector[0];

    float y;

    WindowFilterInit(&y, u.value);

    for (int j = 0; j < sizeof(u_vector) / sizeof(u_vector[0]); j ++) {
        u.value = u_vector[j];

        WindowFilterFunc(&y, &u, 2.0);
        printf("u: %f y: %f\n", u.value, y);

        u.value_z = u.value;
    }

    return 0;
}