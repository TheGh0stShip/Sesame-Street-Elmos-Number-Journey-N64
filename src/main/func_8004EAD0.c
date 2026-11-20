typedef struct { u8 pad[0x44]; s32 id; u8 pad2[0x98-0x48]; s16 unk_98; s16 pad9a; s16 unk_9C; u8 pad3[0xA8-0x9E]; u16 unk_A8; u8 pad4[0x13C-0xAA]; } Ent;
extern s32 D_800A0544;
extern Ent *D_800A054C;
/* set volume-like value on all entries with id; returns count */
s32 func_8004EAD0(s32 id, s32 v) {
    s32 i, n;
    Ent *e;
    if (id == 0) return 0;
    if (v <= 0) v = 1;
    else if (v > 0x100) v = 0x100;
    n = 0;
    e = D_800A054C;
    for (i = 0; i < D_800A0544; i++, e++) {
        if (e->id == id) {
            n++;
            e->unk_98 = v;
            e->unk_9C = (e->unk_A8 * v) >> 7;
        }
    }
    return n;
}
