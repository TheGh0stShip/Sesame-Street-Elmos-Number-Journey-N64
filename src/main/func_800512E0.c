typedef struct {
    u8 pad0[0x24]; f32 unk_24;
    u8 pad28[0xC]; s32 unk_34; s32 unk_38;
    u8 pad3c[0x30]; f32 unk_6C; f32 unk_70;
    u8 pad74[0x2E]; u16 unk_A2; u16 unk_A4;
    u8 padA6[0x16]; u8 unk_BC;
    u8 padBD[0x1E]; u8 unk_DB;
    u8 padDC[4]; s32 unk_E0[4]; s32 unk_F0[4]; s32 unk_100[4];
    u16 unk_110[4]; u16 unk_118[4];
    u8 unk_120[4]; u8 unk_124[4]; f32 unk_128[4];
} S;
/* pop/repeat a loop stack entry */
s32 func_800512E0(S *p, s32 ret) {
    s32 i = p->unk_DB - 1;
    if (p->unk_120[i] != 0xFF) {
        if (--p->unk_120[i] == 0) {
            p->unk_DB = i;
            i = -1;
        }
    }
    if (i >= 0) {
        ret = p->unk_E0[i];
        p->unk_38 = p->unk_F0[i];
        p->unk_34 = p->unk_100[i];
        p->unk_BC = p->unk_124[i];
        p->unk_70 = p->unk_128[i];
        p->unk_A2 = p->unk_110[i];
        p->unk_A4 = p->unk_118[i];
        p->unk_24 = p->unk_70 * p->unk_6C;
    }
    return ret;
}
