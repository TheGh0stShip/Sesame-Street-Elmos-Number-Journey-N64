typedef struct { s32 unk_00; s32 unk_04; u8 unk_08[0x70]; s32 unk_78; u8 unk_7C[0xC0]; } Ent;
extern s32 D_800A0544;
extern Ent *D_800A054C;
/* count active entries matching flag bit 1 (unk_78 set) or bit 2 (unk_78 clear) */
s32 func_8004E804(s32 flags) {
    s32 i;
    s32 n;
    Ent *e;
    i = 0; n = 0;
    e = D_800A054C;
    while (i < D_800A0544) {
        if (e->unk_04 != 0) {
            if (e->unk_78 != 0) {
                if (flags & 1) goto inc;
            } else {
                if (flags & 2) {
inc:
                    n++;
                }
            }
        }
        i++;
        e++;
    }
    return n;
}
