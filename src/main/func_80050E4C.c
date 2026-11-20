/* parses an envelope/rate header from a byte stream into a voice state; returns the advanced pointer */
typedef struct {
    u8 unk_00[0x58];
    f32 unk_58;
    f32 unk_5C;
    f32 unk_60;
    s32 unk_64;
    u8 unk_68[0xBF - 0x68];
    u8 unk_BF;
    u8 unk_C0;
    u8 unk_C1;
    u8 unk_C2;
    u8 unk_C3[3];
    u8 unk_C6;
    u8 unk_C7;
    u8 unk_C8;
} Voice;

u8 *func_80050E4C(Voice *v, u8 *p)
{
    u8 n;
    u8 t;

    n = *p++;
    if (n == 0)
        n = 1;
    v->unk_BF = n;
    v->unk_64 = 0x400 / n;
    v->unk_C0 = *p++;
    n = *p++;
    v->unk_C6 = n;
    t = *p++;
    v->unk_58 = (1.0 / (f32)n) * (f32)(t - v->unk_C0);
    v->unk_C1 = t;
    n = *p++;
    v->unk_C7 = n;
    t = *p++;
    v->unk_5C = (1.0 / (f32)n) * (f32)(t - v->unk_C1);
    v->unk_C2 = t;
    n = *p++;
    v->unk_C8 = n;
    v->unk_60 = 1.0 / (f32)n;
    return p;
}
