#ifndef ANTOINE_EQUATION_H
#define ANTOINE_EQUATION_H

typedef struct {
    float a;
    float b;
    float c;
} antoine_equation_params_t;

float SatTempAntoineEquationCalc(const float absolute_pressure_kpa, const antoine_equation_params_t * antoine_ref_params);

#endif