#ifndef RESET_MANAGER_H
#define RESET_MANAGER_H

#include "stdbool.h"
#define INIT_RESET_INITIAL_VALUE true
#define INIT_RESET_FINAL_VALUE false

typedef struct {

    bool init_reset;
    bool reset_state;

} reset_manager_t;

void ResetManagerInit(reset_manager_t * const reset_manager);

void ResetManagerStep(reset_manager_t * const reset_manager, bool reset);

#endif // RESET_MANAGER_H