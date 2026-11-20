extern char D_8007BDE0[];
f64 func_800694D0(f64);
f64 func_8006A4E0(s32, char *, f64, f64, f64);
/* acos */
f64 func_800695A8(f64 x) {
    if (x > 1.0 || x < -1.0) {
        return func_8006A4E0(1, D_8007BDE0, x, 0.0, 0.0);
    }
    return 1.5707963267948966 - func_800694D0(x);
}
