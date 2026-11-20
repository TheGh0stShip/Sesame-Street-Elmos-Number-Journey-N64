#include "guint.h"
#include "os_version.h"
/* not in the SDK headers */
f32 __sinf(f32);
f32 __cosf(f32);
extern f32 D_80077BD0;
 /* guMtxIdentF */

void guRotateF(f32 mf[4][4], f32 a, f32 x, f32 y, f32 z)
{
    f32 sine, cosine, ab, bc, ca, t;

    guNormalize(&x, &y, &z);
    a *= D_80077BD0;
    sine = __sinf(a);
    cosine = __cosf(a);
    t = (1 - cosine);
    ab = x * y * t;
    bc = y * z * t;
    ca = z * x * t;
    guMtxIdentF(mf);
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
