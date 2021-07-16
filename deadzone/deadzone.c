#include "control_common_code/deadzone.h"

float DeadZone(const float u, const float up_sat_value, const float lo_sat_value) {
    
    float y = 0.0;

    if ( u > up_sat_value)
        y = u - up_sat_value;
    else
    if (u < lo_sat_value)
        y = u - lo_sat_value;
    
    return y;

}