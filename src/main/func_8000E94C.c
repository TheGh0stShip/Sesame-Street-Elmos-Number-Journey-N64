extern s32 D_80072EDC;
extern s32 D_80072ED0;
extern f64 *D_80072EC8;
extern s32 D_80072EC0;

/* Returns a + b * next entry of a cyclic table, or a constant when disabled. */
f64 func_8000E94C(f64 a, f64 b) {
    f64 v;

    if (D_80072EDC == 0) {
        return 1.0;
    }
    v = D_80072EC8[D_80072ED0];
    v = v * b;
    v = v + a;
    D_80072ED0++;
    D_80072ED0 %= D_80072EC0;
    return v;
}
