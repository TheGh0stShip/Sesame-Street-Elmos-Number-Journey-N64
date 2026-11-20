f64 func_80069A90(f64);
/* floor(): truncate, step down for negative non-integers */
f64 func_80069BF0(f64 x) {
    f64 t = func_80069A90(x);
    if ((x < 0.0) & (t != x)) {
        return t - 1.0;
    }
    return t;
}
