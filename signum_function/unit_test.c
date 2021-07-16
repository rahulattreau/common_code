#include <stdio.h>
#include "control_common_code/signum_function.h"

int main() {
    float u_vector[] = {
        -21.0,
        -2.2,
        0.0,
        31.0,
        1.2
    } ;

    for(int j = 0; j < sizeof(u_vector)/sizeof(u_vector[0]); j++) {
        printf("u: %f y: %d\n", u_vector[j], SignumFunction(u_vector[j]));
    }
}