typedef struct {
    u8 pad00[0x30];
    f32 unk_30;
    u8 pad34[0x44 - 0x34];
    s32 unk_44;
    u8 pad48[0x90 - 0x48];
    f32 unk_90;
    u8 pad94[0x9E - 0x94];
    s16 unk_9E;
    u8 padA0[0x13C - 0xA0];
} Obj;
extern s32 D_800A0544;
extern Obj *D_800A054C;
/* Sets unk_9E on every object whose unk_44 equals id; returns how many. */
s32 func_8004E990(s32 id, s16 val) {
    s32 n, i; Obj *o;
    if (id == 0) {
        return 0;
    }
    n = 0;
    for (i = 0, o = D_800A054C; i < D_800A0544; i++, o++) {
        if (o->unk_44 == id) {
            o->unk_9E = val;
            n++;
        }
    }
    return n;
}
