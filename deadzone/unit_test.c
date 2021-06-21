#include <stdio.h>
#include<stdlib.h> // for accessing rand function
#include<time.h> // for accessing time function
#include "deadzone.h"

int main() {

    float u;
    // initialize random number generator
    srand( time(0) );

    for (int j = 0; j < 10; j++) {
        
        u = (float)( (rand() % 40) - 20 ) / 10;
        printf("u: %f y: %f\n", u, DeadZone(u, 1.0, -1.0) );

    }

    // for (int j = 0; j < sizeof(u_vector)/sizeof(u_vector[0]); j++) {
    //     printf("u: %f, y: %f\n", u_vector[j], DeadZone(u_vector[j], 1.0, -1.0));
    // }
    
    return 0;
}