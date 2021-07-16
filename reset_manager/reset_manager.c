#include "reset_manager.h"

void ResetManager_Constructor(reset_manager_t * const reset_manager) {

    reset_manager->init_reset_ = true;

}

void ResetManager_Step(reset_manager_t * const reset_manager, bool reset) {

    reset_manager->reset_state_ = reset_manager->init_reset_ || reset;

    if (reset_manager->init_reset_)
        reset_manager->init_reset_ = false;

}