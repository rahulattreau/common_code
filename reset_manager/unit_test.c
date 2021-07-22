#include "stdio.h"
#include "reset_manager.h"

void print_output(reset_manager_t reset_manager, int j, bool reset) {
    printf("reset: %d step_num %2d init_reset: %d reset_state: %d\n", reset, j, reset_manager.init_reset, reset_manager.reset_state);
}

int main () {

    reset_manager_t reset_manager;
    ResetManagerInit(&reset_manager);
    
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
    
    print_output(reset_manager, -1, reset);

    for (int j = 0; j < u_vector_size; j++) {

        reset = u_vector[j];
        
        ResetManagerStep(&reset_manager, reset);
        print_output(reset_manager, j, reset);
        
    }

    return 0;
}