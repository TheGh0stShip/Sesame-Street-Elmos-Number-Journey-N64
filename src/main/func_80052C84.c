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
void func_8006ADB0(Voice *, s32, void *, s32, s32);
void func_80052E44(u16, s8, u16);
/* allocates a free voice, starts it and returns its handle (0xFFFF if none) */
u16 func_80052C84(s32 arg0, s8 arg1, u16 arg2) {
    s32 i;
    Voice *p;
    for (i = 0; i < D_800A07C4; i++) {
        if (D_800A07C0[i].unk_11E8 == 0xFFFF) {
            p = &D_800A07C0[i];
            p->unk_11E8 = D_800A07C6 & 0xFFF; D_800A07C6++;
            func_8006ADB0(p, arg0, p->unk_11E0, 0xA00, D_800A07CC);
            func_80052E44(D_800A07C0[i].unk_11E8, arg1, arg2);
            D_800A07C0[i].unk_11DE = 4;
            return D_800A07C0[i].unk_11E8;
        }
    }
    return 0xFFFF;
}
