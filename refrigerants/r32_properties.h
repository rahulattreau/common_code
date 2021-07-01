#pragma once

#include "../antoine_equation/antoine_equation.h"

// define refrigerant data for R32

const antoine_equation_params_t r32_antoine_eq_params = {
    .a = 6.52950645,
    .b = 938.96716723, 
    .c = 259.4159998
    };