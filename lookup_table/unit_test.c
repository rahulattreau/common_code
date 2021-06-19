#include<stdio.h>
#include "lookup_table.h"

int main() {

    lookup_table_data_t sat_press_temp_table;
    
    float breakpoints_data[5] = {1.0, 2.0, 3.0, 4.0, 5.0};
    float table_values_data[5] = {2.0, 4.0, 6.0, 8.0, 10.0};
    float y;
    
    // sat_press_temp_table.breakpoints_data = breakpoints_data;
    // sat_press_temp_table.table_values_data = table_values_data;
    // sat_press_temp_table.length = 5;
    sat_press_temp_table = LookupTableTypeInit(breakpoints_data, table_values_data);
    
    for (float j = breakpoints_data[0] - 0.2; j < 5.2; j += 0.1) {
        y = LookupTableFunc(&sat_press_temp_table, j);
        printf("x: %f y: %f \n", j, y);
    }

    for (float j = breakpoints_data[0] - 0.2; j < 5.2; j += 0.1) {
        y = LookupTableFunc(&sat_press_temp_table, j);
        printf("multiple: %f \n", y / j );
    }

    return 0;

}