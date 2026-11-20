extern s32 D_800A07D0;
extern s32 D_800A07D4;
extern s32 D_800A07D8;
extern s32 D_800A07DC;

/* sizes a buffer in 184-byte units and records bounds; returns total */
s32 func_80053620(s32 a, s32 b, u32 c, s32 pct) {
    s32 n;
    s32 m;
    n = ((u32)(a * b + c - 1) / c) / 184;
    n = (n + 1) * 184;
    D_800A07D0 = n;
    D_800A07D4 = n - 184;
    D_800A07D8 = n + 184;
    m = (u32)(n * pct) / 100;
    D_800A07DC = m;
    m += 184;
    return n + m;
}
