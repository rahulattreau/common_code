#include "stdio.h"
#include "saturator.h"

int main () {

    float u_vector[] = {
        0.0,
        -10.0,
        30.0,
        120.0,
        -100.0
    };

    float u;
    float y;
    float upper_limit = 100.0;
    float lower_limit = -10.0;

    for (int j = 0; j < sizeof(u_vector)/sizeof(u_vector[0]); j++ ) {
        u = u_vector[j];
        y = Saturator(u, upper_limit, lower_limit);

        printf("u: %f y: %f \n", u, y);
    }

    return 0;

}