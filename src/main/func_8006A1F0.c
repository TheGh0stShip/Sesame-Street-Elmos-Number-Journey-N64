typedef union {
    f64 d;
    struct {
        s32 hi;
        u32 lo;
    } w;
} du;

typedef union {
    f32 f;
    s32 i;
    s32 k;
} fu;

f64 func_8006A4E0(s32, char *, f64, f64, f64);
extern char D_8007BF00[];

/* square root: float seed from the mantissa, two Newton steps in double, exponent restored by hand */
f64 func_8006A1F0(f64 x) {
    du u;
    du v;
    du r;
    fu s;
    f32 sf;
    fu t;
    s32 e;
    s32 half;
    s32 i;
    s32 k;

    u.d = x;
    v.d = x;
    if (v.w.hi < 0) {
        return func_8006A4E0(1, D_8007BF00, x, 0.0, 0.0);
    }
    e = ((u32)v.w.hi >> 20) & 0x7FF;
    if (e == 0) {
        return 0.0;
    }
    e -= 0x3FF;
    v.w.hi = (v.w.hi & 0x800FFFFF) | 0x3FD00000;
    sf = v.d;
    t.f = sf;
    t.i = (t.i & 0xC07FFFFF) | 0x3D000000;
    if (e & 1) {
        e -= 1;
        half = e >> 1;
        t.f = (f64)(sf + t.f) + 0.4375;
        u.w.hi = (u.w.hi & 0x800FFFFF) | 0x3FE00000;
    } else {
        half = e >> 1;
        t.f = (f64)(sf - t.f) + 0.28125;
        u.w.hi = (u.w.hi & 0x800FFFFF) | 0x3FD00000;
    }
    s.f = u.d;
    t.f = t.f + s.f / t.f;
    k = t.i & 0x3F800000;
    t.i = (t.i & 0xC07FFFFF) | ((k + 0xFF800000) & 0x3F800000);
    r.d = t.f;
    for (i = 0; i < 2; i++) {
        r.d = r.d + u.d / r.d;
        k = r.w.hi & 0x7FF00000;
        r.w.hi = (r.w.hi & 0x800FFFFF) | ((k + 0xFFF00000) & 0x7FF00000);
    }
    r.w.hi = (r.w.hi & 0x800FFFFF) | (((half + 0x3FF) & 0x7FF) << 20);
    return r.d;
}
