#include <stdio.h>
#include "window_filter.h"

int main() {

    float u_vector[] = {1.0, 1.1, 0.9, 1.2, 
    3.1, // big deviation but still within range
    1.2, 1.3, 
    3.5, // out of range
    1.1, 0.9, 1.0, 1.1, 0.9, 1.2, 
    3.1,  // big deviation but still within range
    1.2, 
    3.3, 3.5, // out of range
    1.1, 0.9};
    
    window_filter_t y;
    WindowFilterInit(&y, u_vector[0]);

    for (int j = 0; j < sizeof(u_vector) / sizeof(u_vector[0]); j ++) {
        WindowFilterFunc(&y, u_vector[j], 2.0);
        printf("u: %f y: %f\n", u_vector[j], y.value);
        WindowFilterStoreStateVariable(&y);
    }

    return 0;
}