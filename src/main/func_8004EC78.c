typedef struct { u8 pad[0x44]; s32 id; u8 pad2[0x7C-0x48]; s32 unk_7C; u8 pad3[0x13C-0x80]; } Ent;
extern s32 D_800A0544;
extern Ent *D_800A054C;
/* find entry with id arg0, return its unk_7C */
s32 func_8004EC78(s32 id) {
    s32 i;
    Ent *e;
    if (id == 0) return 0;
    e = D_800A054C;
    for (i = 0; i < D_800A0544; i++, e++) {
        if (e->id == id) return e->unk_7C;
    }
    return 0;
}
