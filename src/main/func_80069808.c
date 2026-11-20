u64 func_8006A990(f64);
void func_80069650(u64, u64, f64 *);

/* Signed wrapper: for |x| > 1e-7 calls func_80069650 on a 2^60 fixed-point ratio (x or 1/x), else returns x. */
f64 func_80069808(f64 x) {
    f64 r;
    s32 sign;
    u64 n, d;
    f64 a;

    if (x < 0.0) {
        x = -x;
        sign = -1;
    } else {
        sign = 1;
    }
    if (x > 1e-7) {
        if (x >= 1.0) {
            f64 c;
            d = 0x1000000000000000ULL;
            a = 1.0 / x;
            c = 1152921504606846976.0;
            a *= c;
            n = func_8006A990(a);
        } else {
            n = 0x1000000000000000ULL;
            d = func_8006A990(x * 1152921504606846976.0);
        }
        func_80069650(n, d, &r);
    } else {
        r = x;
    }
    return (sign < 0) ? -r : r;
}
