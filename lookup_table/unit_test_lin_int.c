#include<stdio.h>
#include "linear_interpolation.h"

int main()
{
    // test linear extrapolation on the low side
    printf("x: %f y: %f \n", 1.5, LinearInterpolation(2.0,3.0,4.0,6.0,1.5) );
    // test linear extrapolation on the high side
    printf("x: %f y: %f \n", 3.5, LinearInterpolation(2.0,3.0,4.0,6.0,3.5) );
    // test linear extrapolation in range
    printf("x: %f y: %f \n", 2.5, LinearInterpolation(2.0,3.0,4.0,6.0,2.5) );

    return 0;

}