typedef struct {
    s32 flags;
    u8 pad4[0xC];
    s32 unk_10;
    u8 pad14[0x8];
    s32 unk_1C;
    u8 pad20[0x24];
    s32 unk_44;
    u8 pad48[0x13C - 0x48];
} Obj;
extern s32 D_800A0544;
extern Obj *D_800A054C;
/* assigns arg1 to idle (-1) objects with id arg0; returns count */
s32 func_8004E884(s32 arg0, s32 arg1) {
    s32 i;
    s32 n;
    s32 v;
    Obj *o;
    if (arg0 == 0) return 0;
    v = arg1;
    if (arg1 == 0) v = 1;
    i = 0;
    o = D_800A054C;
    n = 0;
    for (; i < D_800A0544; i++, o++) {
        if (o->unk_44 == arg0 && o->unk_10 == -1) {
            if (o->flags & 1) {
                o->unk_1C = 1;
                o->unk_10 = 0;
                o->flags &= ~1;
            } else {
                o->unk_1C = v;
                o->unk_10 = arg1;
            }
            n++;
        }
    }
    return n;
}
