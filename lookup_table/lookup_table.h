/* Description:
1. Look-up table with linear inerpolation
2. Extrapolates if values outside of range
*/

// declare lookup_table function prototype
float LookupTableFunc(float const *breakpoints, float const *table_values, const int length, const float x_value);