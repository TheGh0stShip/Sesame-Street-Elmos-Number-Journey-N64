typedef struct {
    u8 unk_0;
    u8 unk_1;
    u8 unk_2;
    u8 unk_3;
    u8 unk_4;
    u8 unk_5;
    u8 unk_6;
} Env; /* size 7 */

typedef struct {
    u8 pad0[0x18];
    Env *unk_18;
} Bank;

typedef struct {
    u8 pad0[0x58];
    f32 unk_58;
    f32 unk_5C;
    f32 unk_60;
    s32 unk_64;
    u8 pad68[0xC];
    Bank *unk_74;
    u8 pad78[0x47];
    u8 unk_BF;
    u8 unk_C0;
    u8 unk_C1;
    u8 unk_C2;
    u8 padC3[3];
    u8 unk_C6;
    u8 unk_C7;
    u8 unk_C8;
} Voice;

/* reads a 1-2 byte envelope index from the stream and loads that envelope's rates */
u8 *func_8004DF20(Voice *v, u8 *p) {
    u32 idx = *p++;
    Env *e;
    s32 n;
    s32 m;

    if (idx & 0x80) {
        idx &= 0x7F;
        idx <<= 8;
        idx |= *p++;
    }
    if ((n = v->unk_74->unk_18[idx].unk_0) == 0) {
        e = &v->unk_74->unk_18[idx];
        n = 1;
    } else {
        e = &v->unk_74->unk_18[idx];
    }
    v->unk_BF = n;
    v->unk_64 = 0x400 / n;
    v->unk_C0 = e->unk_1;
    n = e->unk_2;
    v->unk_C6 = n;
    m = e->unk_3;
    v->unk_58 = 1.0 / (f32)n * (f32)(m - v->unk_C0);
    v->unk_C1 = m;
    n = e->unk_4;
    v->unk_C7 = n;
    m = e->unk_5;
    v->unk_5C = 1.0 / (f32)n * (f32)(m - v->unk_C1);
    v->unk_C2 = m;
    n = e->unk_6;
    v->unk_C8 = n;
    v->unk_60 = 1.0 / (f32)n;
    return p;
}
