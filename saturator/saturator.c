#include "saturator.h"

float Saturator( const float u, float upper_limit, float lower_limit ) {

    float y = u;

    if ( u > upper_limit )
        y = upper_limit;

    else if ( u < lower_limit )
        y = lower_limit;

    return y;
    
}