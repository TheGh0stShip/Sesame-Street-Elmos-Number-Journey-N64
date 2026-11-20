/* libultra cosf */
typedef union { struct { u32 hi, lo; } word; f64 d; } du;
typedef union { u32 i; f32 f; } fu;
static const du P[] = {
    {{0x3ff00000, 0x00000000}}, {{0xbfc55554, 0xbc83656d}}, {{0x3f8110ed, 0x3804c2a0}},
    {{0xbf29f6ff, 0xeea56814}}, {{0x3ec5dbdf, 0x0e314bfe}},
};
static const du rpi = {{0x3fd45f30, 0x6dc9c883}};
static const du pihi = {{0x400921fb, 0x50000000}};
static const du pilo = {{0x3e6110b4, 0x611a6263}};
static const fu zero = {0x00000000};
extern f32 D_8007BAD0; /* __libm_qnan_f */

#define ROUND(d) (s32)(((d) >= 0.0) ? ((d) + 0.5) : ((d) - 0.5))
#define ABS(x) ((x) > 0 ? (x) : -(x))

f32 func_800566A0(f32 x) {
    f32 absx;
    f64 dx, xsq, poly, dn, result;
    s32 n, ix, xpt;

    ix = *(s32 *)&x;
    xpt = (ix >> 22);
    xpt &= 0x1ff;
    if (xpt < 0x136) {
        absx = ABS(x);
        dx = absx;
        dn = dx * rpi.d + 0.5;
        n = ROUND(dn);
        dn = n;
        dn -= 0.5;
        dx = dx - dn * pihi.d;
        dx = dx - dn * pilo.d;
        xsq = dx * dx;
        poly = ((P[4].d * xsq + P[3].d) * xsq + P[2].d) * xsq + P[1].d;
        result = dx + (dx * xsq) * poly;
        if ((n & 1) == 0) {
            return (f32)result;
        }
        return -(f32)result;
    }
    if (x != x) {
        return D_8007BAD0;
    }
    return zero.f;
}
