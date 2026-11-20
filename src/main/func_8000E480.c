extern s32 D_80072EC0;
extern s32 *D_80072EC4;
extern f64 *D_80072EC8;
extern s32 D_80072ECC;
extern s32 D_80072ED0;
extern s32 D_80072ED4;
extern s32 D_80072ED8;
extern s32 D_80072EDC;
extern s32 D_8009D510[];
extern f64 D_8009DD10[];
extern f64 D_8007A030;
s32 func_80069D40(void);
void func_80069D74(s32);
f64 func_8006A1F0(f64);

/* seeds the RNG and fills tables of random integers and clamped random doubles */
s32 func_8000E480(s32 arg0, s32 arg1) {
    f64 a;
    f64 scale;
    s32 sum;
    s32 i;
    s32 k;

    D_80072EC0 = 0x200;
    D_80072EC4 = D_8009D510;
    D_80072EC8 = D_8009DD10;
    D_80072ED0 = 0;
    D_80072ECC = 0;
    D_80072ED8 = 0;
    D_80072ED4 = 0;
    func_80069D74(arg0);
    for (i = 0; i < D_80072EC0; i++) {
        D_80072EC4[i] = abs(func_80069D40() % 32767);
    }
    a = func_8006A1F0(D_8007A030);
    scale = 2.0 * a / 983010.0;
    for (i = 0; i < D_80072EC0; i++) {
        do {
            sum = 0;
            for (k = 0; k < 30; k++) {
                sum += abs(func_80069D40() % 32767);
            }
            D_80072EC8[i] = sum * scale - a;
        } while (D_80072EC8[i] < -0.95 || D_80072EC8[i] > 0.95);
    }
    D_80072EDC = 1;
    return arg0;
}
