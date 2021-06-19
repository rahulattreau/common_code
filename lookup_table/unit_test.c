#include<stdio.h>
#include "lookup_table.h"

int main() {

    // define lookup table data
    float breakpoints_data[5] = {1.0, 2.0, 3.0, 4.0, 5.0};
    float table_values_data[5] = {2.0, 4.0, 6.0, 8.0, 10.0};
    float y;
    
    lookup_table_data_t sat_press_temp_table;
    LookupTable__Constructor(&sat_press_temp_table, breakpoints_data, table_values_data);
    
    printf("x: %f y: %f \n", sat_press_temp_table.breakpoints_data[0], sat_press_temp_table.table_values_data[2]);
    for (float j = breakpoints_data[0] - 0.2; j < 5.2; j += 0.1) {
        y = LookupTable__Func(&sat_press_temp_table, j);
        printf("x: %f y: %f \n", j, y);
    }

    for (float j = breakpoints_data[0] - 0.2; j < 5.2; j += 0.1) {
        y = LookupTable__Func(&sat_press_temp_table, j);
        printf("multiple: %f \n", y / j );
    }

    return 0;

}