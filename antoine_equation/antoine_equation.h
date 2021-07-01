#pragma once

typedef struct {
    float a;
    float b;
    float c;
} antoine_equation_params_t;

float SatTempAntoineEquationCalc(
    const float absolute_pressure_kpa, 
    const antoine_equation_params_t * const antoine_ref_params
    );