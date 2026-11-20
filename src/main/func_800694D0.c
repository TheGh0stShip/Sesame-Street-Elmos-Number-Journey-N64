extern char D_8007BDC0[];
double sqrt(double);
f64 func_80069920(f64, f64);
void func_8006A4E0(s32, char *, f64, f64, f64);
/* arcsine: range check then atan2(x, sqrt(1-x*x)) */
f64 func_800694D0(f64 x) {
    if ((x > 1.0) | (x < -1.0)) {
        func_8006A4E0(1, D_8007BDC0, x, 0.0, 0.0);
        return;
    }
    return func_80069920(x, sqrt(1.0 - x * x));
}
