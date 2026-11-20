typedef struct { u8 pad[0x10]; s32 unk_10; u8 pad1[0x1C-0x14]; u32 unk_1C; u8 pad2[0x78-0x20]; s32 unk_78;
  u8 pad3[0x9E - 0x7C]; s16 unk_9E; u16 unk_A0; u8 pad4[0xB0-0xA2]; s16 unk_B0; u8 pad5[0xBB-0xB2];
  u8 unk_BB, unk_BC, unk_BD, unk_BE; u8 pad6[0xC4-0xBF]; u8 unk_C4; } Chan;
typedef struct { u8 b[0x1C]; } Syn;
extern Syn *D_800A0548;
extern s32 D_800A0558;
extern u16 D_800A055C;
extern u16 D_800A055E;
void func_80055E50(Syn *, s16, s32);
void func_80055D30(Syn *, u8);
/* update channel volume and pan */
void func_8004F990(Chan *c, s32 idx) {
    u32 v;
    v = (u32)(c->unk_BC * c->unk_C4 * c->unk_BB * c->unk_9E) >> 13;
    if (v > 0x7FFF) v = 0x7FFF;
    if (c->unk_78 == 0) v *= D_800A055E; else v *= D_800A055C;
    v >>= 15;
    if (c->unk_10 != -1) v = v * c->unk_10 / c->unk_1C;
    if (v != c->unk_A0) {
        c->unk_A0 = v;
        func_80055E50(&D_800A0548[idx], v, D_800A0558);
    }
    v = ((c->unk_BD * c->unk_B0) >> 7) & 0x7F;
    if (v != c->unk_BE) {
        c->unk_BE = v;
        func_80055D30(&D_800A0548[idx], v);
    }
}
