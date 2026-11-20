typedef struct { u8 pad[0x14]; s32 unk_14; } SubB;
typedef struct { u8 pad[0x20]; s32 unk_20; } SubA;
typedef struct {
    u8 pad0[0x44]; s32 unk_44;
    u8 pad48[0x74 - 0x48]; SubA *unk_74; SubB *unk_78;
    u8 pad7C[0xCB - 0x7C]; s8 unk_CB; u8 unk_CC;
    u8 padCD[0x13C - 0xCD];
} Entry;
extern s32 D_800A0544;
extern Entry *D_800A054C;
/* sets value (clamped 0..127) on all entries with id, marks them; returns count */
s32 func_8004EB5C(s32 id, s32 val) {
    s32 i, n;
    Entry *e;
    if (id == 0) return 0;
    if (val < 0) val = 0;
    else if (val >= 0x80) val = 0x7F;
    n = 0;
    e = D_800A054C;
    for (i = 0; i < D_800A0544; i++, e++) {
        if (e->unk_44 == id) {
            n++;
            e->unk_CB = val;
            e->unk_CC = 0xFF;
        }
    }
    return n;
}
