#include "stdbool.h"

typedef struct {

    bool init_reset_;
    bool reset_state_;

} reset_manager_t;

void ResetManager_Constructor(reset_manager_t * const reset_manager);

void ResetManager_Step(reset_manager_t * const reset_manager, bool reset);