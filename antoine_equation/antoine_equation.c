#include <math.h>
#include "antoine_equation.h"

float SatTempAntoineEquationCalc(const float absolute_pressure_kpa, const antoine_equation_params_t * antoine_ref_params) {
    
    // sat_temp = B / ( A - log10(absolute_pressure_kpa) ) - C

    float sat_temp = antoine_ref_params->b / 
    ( antoine_ref_params->a - log10f(absolute_pressure_kpa )) 
    - antoine_ref_params->c;

    return sat_temp;
}