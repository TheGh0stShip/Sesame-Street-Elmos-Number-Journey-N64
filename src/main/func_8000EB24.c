extern s32 D_80072EC0;
extern f64 *D_80072EC8;
extern s32 D_80072ED8;
extern s32 D_80072EDC;
extern f64 D_8007A058;

/* Returns base + scale * next value from a cyclic table, or a default when disabled. */
f64 func_8000EB24(f64 base, f64 scale) {
    f64 v;

    if (D_80072EDC == 0) {
        return D_8007A058;
    }
    v = D_80072EC8[D_80072ED8];
    v = v * scale;
    v = v + base;
    D_80072ED8++;
    D_80072ED8 = D_80072ED8 % D_80072EC0;
    return v;
}
