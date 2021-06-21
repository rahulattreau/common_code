signed char SignumFunction(const float u) {
    /*
    Description:
    1. Takes a float number as an input.
    2. Returns an 8-bit signed integer:
        +1 = positive number
        0 = zero
        -1 = negative number
    */

    signed char y = 0;
    if ( u > 0.0 ) 
        y = 1;
    else
    if ( u < 0.0 )
        y = -1;

    return y;
}
