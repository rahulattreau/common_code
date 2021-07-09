#pragma once
#include <stdbool.h>

// define private variables
typedef struct {
    float value_;
} unit_delay_private_t;

// define public variables
typedef struct {
    float yk_;
    float xk_1_;
} unit_delay_t;

void UnitDelay_Constructor(unit_delay_t * const instance, const float init_value);

void UnitDelay_StoreInternalState(unit_delay_t * const instance, const float xk);

void UnitDelay_Step(unit_delay_t * const instance, const float xk, const bool reset);