/*
Description:
1. Returns a value that is zero within a range.
2. Subtracts the upper/lower limit from the input when outside the range.

Inputs:
1. u - input value on which the deadzone needs to be calculated
2. up_sat_value - upper value for the deadzone
3. lo_sat_value - lower value for the deadzone

Returns: A float on which the deadzone has been performed.
*/
    
// declare deadzone function type
float DeadZone(const float u, const float up_sat_value, const float lo_sat_value);