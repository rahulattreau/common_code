#ifndef DIFFERENTIATOR_H
#define DIFFERENTIATOR_H

#include <stdbool.h>
#include "unit_delay.h"

typedef struct {
    double yk_;
    unit_delay_t yk_1_;
    float time_step_;
    
} differentiator_t;

void Differentiator_Constructor(differentiator_t * const instance, const float time_step);

void Differentiator_Step(differentiator_t * const instance, const float u, const bool reset);

#endif // DIFFERENTIATOR_H