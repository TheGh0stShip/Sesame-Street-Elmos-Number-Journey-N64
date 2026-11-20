/* _frexpf (env.c): split value into mantissa in [0.5,1) and binary exponent */
f64 func_8005B268(f64 value, s32 *eptr)
{
    f64 absvalue;

    *eptr = 0;
    if (value == 0.0) /* nothing to do for zero */
        return (value);
    absvalue = (value > 0.0) ? value : -value;
    for ( ; absvalue >= 1.0; absvalue *= 0.5)
        ++*eptr;
    for ( ; absvalue < 0.5; absvalue += absvalue)
        --*eptr;
    return (value > 0.0 ? absvalue : -absvalue);
}
