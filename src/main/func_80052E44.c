typedef struct { u8 pad[0x11E0]; void *unk_11E0; void *unk_11E4; u16 unk_11E8; u8 pad2[0x11F8 - 0x11EA]; u16 unk_11F8; u16 unk_11FA; u8 pad3[0x1200 - 0x11FC]; } Voice;
extern Voice *D_800A07C0;
extern u16 D_800A07C4;
void func_80052E44(u16 id, s8 pan, u16 vol) {
    s32 i;
    if (id == 0xFFFF) return;
    for (i = 0; i < D_800A07C4; i++) {
        if (D_800A07C0[i].unk_11E8 == id) {
            if ((u8)pan < 64) {
                D_800A07C0[i].unk_11F8 = (u32)(vol * (pan * 0.25f / 64.0f + 0.75f));
                D_800A07C0[i].unk_11FA = (u32)(vol * (pan * 0.75f / 64.0f));
            } else if (pan > 64) {
                D_800A07C0[i].unk_11F8 = (u32)(vol * ((127 - pan) * 0.75f / 63.0f));
                D_800A07C0[i].unk_11FA = (u32)(vol * ((pan - 64) * 0.25f / 63.0f + 0.75f));
            } else {
                D_800A07C0[i].unk_11F8 = (u32)(vol * 0.75f);
                D_800A07C0[i].unk_11FA = (u32)(vol * 0.75f);
            }
        }
    }
}
