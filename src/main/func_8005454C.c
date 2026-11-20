/* _getRate (env.c, older linear form): rate integer part returned, fraction in *ratel */
s16 func_8005454C(f64 vol, f64 tgt, s32 count, u16 *ratel)
{
    s16 s, s2;
    f64 invn, a;

    if (count == 0) {
        if (tgt >= vol) {
            *ratel = 0xFFFF;
            return 0x7FFF;
        } else {
            *ratel = 0;
            return -0x8000;
        }
    }
    invn = 1.0 / count;
    if (tgt < 1.0)
        tgt = 1.0;
    if (vol <= 0)
        vol = 1.0;
    a = (tgt - vol) * invn * 8.0;
    s = (s16)a;
    a -= s;
    a += 1.0;
    s2 = (s16)a;
    a -= s2;
    s -= 1;
    s += s2;
    *ratel = (u16)(a * 65535.0);
    return s;
}
