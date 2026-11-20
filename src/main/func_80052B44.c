typedef struct {
    u8 pad0[0x11A2]; u16 unk_11A2;
    u8 pad11A4[0x11DD - 0x11A4]; u8 unk_11DD; u8 unk_11DE; u8 pad11DF;
    void *unk_11E0; u8 pad11E4[4];
    u16 unk_11E8; u8 unk_11EA; u8 pad11EB;
    s32 unk_11EC; f32 unk_11F0; s32 unk_11F4;
    u8 pad11F8[0x1200 - 0x11F8];
} Voice;
extern Voice *D_800A07C0;
extern u16 D_800A07C4;
extern u16 D_800A07C6;
extern s32 D_800A07C8;
extern s32 D_800A07CC;
void func_8006AE74(Voice *);
void func_8006B258(Voice *);
/* per-voice update: restart voices in state 2, free voices in state 4 */
void func_80052B44(void) {
    s32 i;
    Voice *p;
    for (i = 0; i < D_800A07C4; i++) {
        if (D_800A07C0[i].unk_11E8 != 0xFFFF) {
            func_8006AE74(&D_800A07C0[i]);
            switch (D_800A07C0[i].unk_11DD) {
            case 2:
                p = &D_800A07C0[i];
                p->unk_11EA = 1;
                p->unk_11EC = 0;
                D_800A07C0[i].unk_11F4 = 0;
                D_800A07C0[i].unk_11F0 = (f32)((D_800A07C0[i].unk_11A2 << 15) / D_800A07C8) / 32768.0f;
                func_8006B258(&D_800A07C0[i]);
                break;
            case 4:
                D_800A07C0[i].unk_11E8 = 0xFFFF;
                break;
            }
        }
    }
}
