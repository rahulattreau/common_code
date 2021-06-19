#include<stdio.h>
#include "lookup_table.h"

int main()

{
    float breakpoints_data[5] = {1.0, 2.0, 3.0, 4.0, 5.0};
    float table_values_data[5] = {2.0, 4.0, 6.0, 8.0, 10.0};
    
    for (float j = breakpoints_data[0] - 0.1; j < 5.1; j += 0.1) {
        printf("x: %f y: %f \n", j, LookupTableFunc(breakpoints_data, table_values_data, 5, j) );
    }

    return 0;

}