typedef struct {
    s32 unk_0;
    s32 unk_4;
    u8 unk_8[0x8];
    s32 unk_10;
    u8 unk_14[0x8];
    s32 unk_1C;
    u8 unk_20[0x58];
    s32 unk_78;
    u8 unk_7C[0xC0];
} Obj; /* size 0x13C */

extern s32 D_800A0544;
extern Obj *D_800A054C;

/* for objects selected by mask bits (1: with unk_78 set, 2: without) that are active and have unk_10 == -1, assign arg */
void func_8004E738(s32 mask, s32 arg) {
    s32 v;
    s32 i;
    Obj *o;

    v = arg;
    if (arg == 0) {
        v = 1;
    }
    o = D_800A054C;
    for (i = 0; i < D_800A0544; i++, o++) {
        if ((o->unk_78 != 0) ? !(mask & 1) : !(mask & 2)) {
            continue;
        }
        if (o->unk_4 != 0 && o->unk_10 == -1) {
            if (o->unk_0 & 1) {
                o->unk_1C = 1;
                o->unk_10 = 0;
                o->unk_0 &= ~1;
            } else {
                o->unk_1C = v;
                o->unk_10 = arg;
            }
        }
    }
}
