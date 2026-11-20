typedef struct { u8 pad[0x11E0]; void *unk_11E0; void *unk_11E4; u16 unk_11E8; u8 pad2[0x1200-0x11EA]; } Voice;
void *func_8005293C(s32);
extern Voice *D_800A07C0;
extern s16 D_800A07C4;
extern s16 D_800A07C6;
extern s32 D_800A07C8;
extern void *D_800A07CC;
extern u8 D_80053574[];
/* allocate n voice blocks with buffers */
s32 func_80052A30(s32 n, s32 arg1, s32 arg2, void *cb) {
    s32 i;
    D_800A07C4 = 0;
    D_800A07C6 = 0;
    D_800A07C8 = arg1;
    D_800A07C0 = func_8005293C(n * sizeof(Voice));
    if (D_800A07C0 == NULL) return 0;
    for (i = 0; i < n; i++) {
        D_800A07C0[i].unk_11E8 = 0xFFFF;
        if ((D_800A07C0[i].unk_11E0 = func_8005293C(0x1400)) == NULL) return 0;
        if ((D_800A07C0[i].unk_11E4 = func_8005293C(0x20)) == NULL) return 0;
    }
    D_800A07C4 = n;
    D_800A07CC = cb;
    if (cb == NULL) D_800A07CC = D_80053574;
    return 1;
}
