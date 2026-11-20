typedef struct {
    f64 v[8];
} Log2Table;

extern Log2Table D_8007B950;

/* Splits x into a mantissa in [0.5, 1) and a power-of-two exponent. */
static inline f64 splitExp(f64 x, s32 *exp) {
    f64 m;

    *exp = 0;
    if (x == 0.0) {
        return x;
    }
    m = (x > 0.0) ? x : -x;
    while (m >= 1.0) {
        m *= 0.5;
        (*exp)++;
    }
    while (m < 0.5) {
        m += m;
        (*exp)--;
    }
    return (x > 0.0) ? m : -m;
}

/* Computes (b/a)^(1/n) with a table-driven log2, as 16.16 fixed point: integer part returned, fraction in *frac. */
s16 func_8005B36C(f64 a, f64 b, s32 n, u16 *frac) {
    Log2Table tbl;
    s32 exp;
    s32 k;
    f64 r, x, y;
    f64 inv = 1.0 / n;
    s16 ip;

    if (n == 0) {
        if (a <= b) {
            *frac = 0xFFFF;
            return 0x7FFF;
        }
        *frac = 0;
        return 0;
    }
    if (b < 1.0) {
        b = 1.0;
    }
    if (a <= 0.0) {
        a = 1.0;
    }
    tbl = D_8007B950;
    k = inv * 1073741824.0;
    r = splitExp(b / a, &exp);
    x = (tbl.v[(s32) (r * 16.0) - 8] + exp) * 0.6931471805599453 / 1073741824.0 + 1.0;
    y = 1.0;
    while (k != 0) {
        if (k & 1) {
            y *= x;
        }
        k >>= 1;
        x *= x;
    }
    y *= y;
    y *= y;
    y *= y;
    ip = y;
    *frac = (s32) ((y - (f32) ip) * 65535.0);
    return ip;
}
