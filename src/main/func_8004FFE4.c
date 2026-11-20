typedef struct {
    u8 unk_00[0xC]; s32 unk_0C; u8 unk_10[4]; s32 unk_14;
    u8 unk_18[0x38 - 0x18]; u8 *unk_38; u8 unk_3C[0xA2 - 0x3C]; u16 unk_A2;
    u8 unk_A4[0xBC - 0xA4]; u8 unk_BC;
} Seq;
/* advances a byte-coded note sequence up to the current time */
void func_8004FFE4(Seq *s) {
    u8 b;
    do {
        s->unk_14 += 0x100;
        if (--s->unk_A2 == 0) {
            b = *s->unk_38++;
            if (b >= 0x80) {
                s->unk_BC = b & 0x7F;
                b = *s->unk_38++;
                if (b >= 0x80) {
                    s->unk_A2 = (b & 0x7F) << 8;
                    s->unk_A2 += *s->unk_38++ + 2;
                } else {
                    s->unk_A2 = b + 2;
                }
            } else {
                s->unk_BC = b;
                s->unk_A2 = 1;
            }
        }
    } while (s->unk_14 - s->unk_0C < 0);
}
