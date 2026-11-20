extern f64 D_8007B518;
extern f64 D_8007B520;
extern f64 D_8007B528;
extern f64 D_8007B530;
extern f64 D_8007B538;
extern f64 D_8007B540;
extern f64 D_8007B548;
extern f64 D_8007B550;
extern f64 D_8007B558;
extern f64 D_8007B560;
extern f64 D_8007B568;
extern f64 D_8007B570;
extern f64 D_8007B578;
extern f64 D_8007B580;

/* 2^x by ln2 power series; 1/2^-x for negative x */
f32 func_800501BC(f32 x) {
    f32 x2, x4, y4;
    f64 r;
    if (x == 0.0f) return 1.0f;
    if (x > 0.0f) {
        x2 = x * x;
        return (f64)x * D_8007B550 + D_8007B570 + (f64)x2 * D_8007B558 + (f64)(x2 * x) * D_8007B560
          + (f64)(x2 * x2) * D_8007B568 + (f64)(x2 * x2 * x) * D_8007B578 + (f64)(x2 * x2 * x2) * D_8007B580;

    } else {
        x = -x;
        x2 = x * x;
        y4 = x2 * x2;
        return D_8007B538 / ((f64)x * D_8007B518 + D_8007B538 + (f64)x2 * D_8007B520 + (f64)(x2 * x) * D_8007B528
          + (f64)y4 * D_8007B530 + (f64)(y4 * x) * D_8007B540 + (f64)(y4 * x2) * D_8007B548);
    }
}
