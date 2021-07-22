#include "reset_manager.h"

void ResetManagerInit(reset_manager_t * const reset_manager) {

    reset_manager->init_reset = INIT_RESET_INITIAL_VALUE;

}

void ResetManagerStep(reset_manager_t * const reset_manager, bool reset) {

    reset_manager->reset_state = reset_manager->init_reset || reset;

    if (reset_manager->init_reset)
        reset_manager->init_reset = INIT_RESET_FINAL_VALUE;

}