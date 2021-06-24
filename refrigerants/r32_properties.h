#ifndef R32_PROPERTIES_H
#define R32_PROPERTIES_H

#include "../antoine_equation/antoine_equation.h"

// define refrigerant data for R32

const antoine_equation_params_t r32_antoine_eq_params = {
    .a = 6.48393549, 
    .b = 916.25497262, 
    .c = 256.38571
    };

#endif