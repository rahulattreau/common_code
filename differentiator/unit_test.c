#include <stdio.h>
#include "differentiator.h"

int main () {

    const float time_step = 0.1;
    differentiator_t differentiator;
    Differentiator_Constructor( &differentiator, time_step);

    float u_vector[] = {-1.0, 0.0, 0.5, 2.0, -0.5};
    float u;
    bool reset = false;

    float time_value = 0;
    for (int j = 0; j < (sizeof(u_vector)/sizeof(u_vector[0])); j ++ ) {

        u = u_vector[j];
        Differentiator_Step( &differentiator, u, reset);

        printf("j: %2d u: %f y: %f\n", j, u, differentiator.yk_);
        time_value += time_step;
    }

    return 0;
}