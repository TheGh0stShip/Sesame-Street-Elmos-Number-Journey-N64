typedef struct {
    u8 unk_0[0x44];
    s32 unk_44;
    u8 unk_48[0x68];
    s16 unk_B0;
    u8 unk_B2[0xC];
    u8 unk_BE;
    u8 unk_BF[0x7D];
} Obj; /* size 0x13C */

extern s32 D_800A0544;
extern Obj *D_800A054C;

/* sets unk_B0 and marks unk_BE on every object whose unk_44 equals id; returns how many */
s32 func_8004E9F0(s32 id, s16 val) {
    s32 i;
    s32 n;
    Obj *o;

    if (id == 0) {
        return 0;
    }
    n = 0;
    o = D_800A054C;
    for (i = 0; i < D_800A0544; i++, o++) {
        if (o->unk_44 == id) {
            n++;
            o->unk_B0 = val;
            o->unk_BE = 0xFF;
        }
    }
    return n;
}
