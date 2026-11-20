typedef struct { u8 pad[0x11E8]; u16 unk_11E8; u8 pad2[0x1200-0x11EA]; } Ent;
extern Ent *D_800A07C0;
extern u16 D_800A07C4;
/* is an entity with id arg0 present */
s32 func_80052DE4(u16 arg0) {
    s32 i;
    if (arg0 == 0xFFFF) return 0;
    for (i = 0; i < D_800A07C4; i++) {
        if (D_800A07C0[i].unk_11E8 == arg0) return 1;
    }
    return 0;
}
