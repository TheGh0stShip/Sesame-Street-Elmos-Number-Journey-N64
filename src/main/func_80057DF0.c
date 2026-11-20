/* guScale (guScaleF inlined) */
typedef struct { s32 m[4][4]; } Mtx;
extern void func_80057040(f32 mf[4][4]);            /* guMtxIdentF */
extern void func_80056F00(f32 mf[4][4], Mtx *m);    /* guMtxF2L */
static inline void guScaleF(f32 mf[4][4], f32 x, f32 y, f32 z)
{
    func_80057040(mf);
    mf[0][0] = x;
    mf[1][1] = y;
    mf[2][2] = z;
    mf[3][3] = 1;
}
void func_80057DF0(Mtx *m, f32 x, f32 y, f32 z)
{
    f32 mf[4][4];

    guScaleF(mf, x, y, z);
    func_80056F00(mf, m);
}
