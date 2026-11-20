/* guRotateF */
extern f32 D_80077BD0; /* dtor */
void func_80061D30(f32 *x, f32 *y, f32 *z); /* guNormalize */
f32 func_80057E70(f32); /* sinf */
f32 func_800566A0(f32); /* cosf */
void func_80057040(f32 mf[4][4]); /* guMtxIdentF */

void func_80057790(f32 mf[4][4], f32 a, f32 x, f32 y, f32 z)
{
    f32 sine, cosine, ab, bc, ca, t;

    func_80061D30(&x, &y, &z);
    a *= D_80077BD0;
    sine = func_80057E70(a);
    cosine = func_800566A0(a);
    t = (1 - cosine);
    ab = x * y * t;
    bc = y * z * t;
    ca = z * x * t;
    func_80057040(mf);
    t = x * x;
    mf[0][0] = t + cosine * (1 - t);
    mf[2][1] = bc - x * sine;
    mf[1][2] = bc + x * sine;
    t = y * y;
    mf[1][1] = t + cosine * (1 - t);
    mf[2][0] = ca + y * sine;
    mf[0][2] = ca - y * sine;
    t = z * z;
    mf[2][2] = t + cosine * (1 - t);
    mf[1][0] = ab - z * sine;
    mf[0][1] = ab + z * sine;
}
