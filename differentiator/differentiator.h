#ifndef DIFFERENTIATOR_H
#define DIFFERENTIATOR_H

#include <stdbool.h>
#include "../unit_delay/control_common_code/unit_delay.h"

typedef struct {
    double output;
    unit_delay_t output_k_1;
    float time_step;
    
} differentiator_t;

void DifferentiatorInit(differentiator_t * const instance, const float time_step);

void DifferentiatorStep(differentiator_t * const instance, const float u, const bool reset);

#endif // DIFFERENTIATOR_H