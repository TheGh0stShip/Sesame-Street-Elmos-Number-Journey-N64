typedef struct {
    u8 unk_0[0x10];
    s32 unk_10;
    u8 unk_14[0x4];
    struct { s32 unk_0; s32 unk_4; } unk_18[1];
} Def;

typedef struct {
    u8 unk_0[0x4];
    s32 unk_4;
    u8 unk_8[0x3C];
    s32 unk_44;
    s32 unk_48;
    u8 unk_4C[0x2C];
    Def *unk_78;
    s32 unk_7C;
    s32 unk_80;
    u8 unk_84[0x1A];
    s16 unk_9E;
    u8 unk_A0[0x6];
    s16 unk_A6;
    u8 unk_A8[0x8];
    s16 unk_B0;
} Obj;

extern s32 D_800A0560;
void func_8005057C(void);

/* initialises an object from a definition and assigns it the next serial id, which it returns */
s32 func_800508B8(Obj *o, Def *d, s32 idx, s32 a3, s32 a4, s32 a5) {
    func_8005057C();
    o->unk_A6 = idx;
    o->unk_78 = d;
    o->unk_9E = a3;
    o->unk_B0 = a4;
    o->unk_44 = D_800A0560++;
    o->unk_48 = a5;
    if (d->unk_10 != 0) {
        o->unk_7C = d->unk_10;
    }
    o->unk_4 = o->unk_80 = d->unk_18[idx].unk_0;
    return o->unk_44;
}
