extern s32 D_80072EC0;
extern s32 D_80072EC4;
extern s32 D_80072ED4;
extern s32 D_80072EDC;

s32 f(s32 arg0) {
    s32 sp0;

    if (D_80072EDC == 0) {
        return -1;
    }
    sp0 = (*(s32 *)((D_80072ED4 * 4) + D_80072EC4)) % arg0;
    D_80072ED4++;
    D_80072ED4 = D_80072ED4 % D_80072EC0;
    return sp0;
}
