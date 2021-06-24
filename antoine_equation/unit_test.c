#include "stdio.h"
#include "antoine_equation.h"

int main() {

    const antoine_equation_params_t r32_antoine_eq_params[] = {
        6.48393549, 
        916.25497262, 
        256.38571
        };
    
    float u_vector[] = {200.0, 500.0, 800.0, 1100.0, 1400.0, 1700.0, 2000.0, 2300.0, 2700.0, 3200.0}; // kPa
    float y_vector[] = {
        -37.323247616872266,
        -14.3305862303298,
        -0.5061267386433883,
        9.789556940152181,
        18.13794475860908,
        25.228879633734948,
        31.430702688088786,
        36.96520672300255,
        43.54983961641517,
        50.80555953133944
        };

    for (int j = 0; j < sizeof(u_vector)/sizeof(u_vector[0]); j++) {
        float y = SatTempAntoineEquationCalc(u_vector[j], &r32_antoine_eq_params[0]);
        printf("pressure, kPa: %f, sat temp, deg C: %f, error: %f\n", u_vector[j], y, y_vector[j] - y);
    }
    
    return 0;
}