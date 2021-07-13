#include "../feedback_loop/feedback_loop.h"

typedef struct {
    feedback_loop_t loop_;
    float time_step_;
} integrator_t;

void Integrator_Constructor(integrator_t * const instance, const float time_step);

void (*Integrator_Init)(feedback_loop_t * const instance, float xk);

void (*Integrator_Step)(feedback_loop_t * const instance, const float xk, const bool reset);