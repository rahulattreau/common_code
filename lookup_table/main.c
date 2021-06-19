#include<stdio.h>
#include "lookup_table.h"

int main()

{
    float breakpoints_data[5] = {1.0, 2.0, 3.0, 4.0, 5.0};
    float table_values_data[5] = {2.0, 4.0, 6.0, 8.0, 10.0};
    // printf("\nThe value of y is: ");
    
    // printf("%f", lookup_table(breakpoints_data, table_values_data, 5, 4.0) );
    // printf("\n");

    for (float j = breakpoints_data[0]; j < 5.0; j += 0.1) {
        printf("x: %f y: %f \n", j, lookup_table(breakpoints_data, table_values_data, 5, j) );
    }

    return 0;

}