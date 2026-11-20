extern s64 D_80079010[];
f64 func_8006AAB0(s64);

/* CORDIC vectoring: accumulate the angle of (x, y) in fixed point 2^60, store as double */
void func_80069650(s64 x0, s64 y, f64 *out) {
    s64 x = x0;
    s64 angle = 0;
    s64 nx;
    s32 i;
    for (i = 0; i < 52; i++) {
        if (y >= 0) {
            nx = x + (y >> i);
            y -= x >> i;
            angle += D_80079010[i];
        } else {
            nx = x - (y >> i);
            y += x >> i;
            angle -= D_80079010[i];
        }
        x = nx;
    }
    *out = func_8006AAB0(angle) / 1.152921504606847e+18;
}
