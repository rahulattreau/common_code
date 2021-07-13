#include "../feedback_loop/feedback_loop.h"

typedef struct {
    feedback_loop_t feedback_loop; 
} accumulator_t;

void Accumulator_Constructor(accumulator_t * const instance);

void (*Accumulator_Init)(feedback_loop_t * const instance, float xk);

void (*Accumulator_Step)(feedback_loop_t * const instance, const float xk, const bool reset);