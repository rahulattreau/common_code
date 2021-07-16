#include "stdio.h"
#include "reset_manager.h"

void print_output(reset_manager_t reset_manager, bool reset) {
    printf("reset: %d init_reset: %d reset_state: %d\n", reset, reset_manager.init_reset_, reset_manager.reset_state_);
}

int main () {

    reset_manager_t reset_manager;
    ResetManager_Constructor(&reset_manager);
    
    // create the u_vector
    int u_vector_size = 10;
    bool u_vector[u_vector_size];
    for (int j = 0; j < u_vector_size; j++) {
        u_vector[j] = false;
    }

    u_vector[4] = true;
    u_vector[5] = true;
    u_vector[9] = true;
    
    // end of create the u_vector

    bool reset = false;
    
    for (int j = 0; j < u_vector_size; j++) {

        reset = u_vector[j];
        
        ResetManager_Step(&reset_manager, reset);
        print_output(reset_manager, reset);
        
    }

    return 0;
}