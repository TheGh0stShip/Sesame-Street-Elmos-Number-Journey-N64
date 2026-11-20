typedef struct {
    u8 pad0[0xC];
    s32 unk_C;
    u8 pad10[0x94 - 0x10];
    s32 unk_94;
    u8 pad98[0xBD - 0x98];
    u8 unk_BD;
    u8 padBE[0xD4 - 0xBE];
    u8 unk_D4;
    u8 padD5[4];
    u8 unk_D9;
    u8 unk_DA;
} S;
/* advances a ramping 7-bit value (unk_BD) up and down until unk_94 catches up to unk_C */
void func_8004FE70(S *p) {
    u32 v;

    do {
        v = p->unk_D9 + p->unk_D4;
        p->unk_94 += 0x100;
        if (v < 0x40) {
            p->unk_D9 = v;
        } else {
            p->unk_D9 = v & 0x3F;
            v >>= 6;
            if (p->unk_DA == 0) {
                p->unk_BD += v;
                if (p->unk_BD >= 0x80) {
                    p->unk_BD = 0x7F;
                    p->unk_DA = 1;
                }
            } else {
                p->unk_BD -= v;
                if (p->unk_BD >= 0x80 || p->unk_BD == 0) {
                    p->unk_BD = 0;
                    p->unk_DA = 0;
                }
            }
        }
    } while (p->unk_94 - p->unk_C < 0);
}
