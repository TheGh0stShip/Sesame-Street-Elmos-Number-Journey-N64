typedef struct { s32 unk_00; u8 pad[0x10]; s32 unk_14; } S;
/* parameter setter switch: 1 -> unk_00, 6 -> unk_14 */
s32 func_8005C6F8(S *p, s32 type, s32 value) {
    switch (type) {
    case 1:
        p->unk_00 = value;
        break;
    case 6:
        p->unk_14 = value;
        break;
    }
    return 0;
}
