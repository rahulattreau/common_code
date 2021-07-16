/* 
description:
1. this is a saturator, it returns the same value in the range, a limited value outside the range
2. limit values are part of the acceptable range, i.e. range = [lower limit, upper limit]
*/

float Saturator( const float u, float upper_limit, float lower_limit );