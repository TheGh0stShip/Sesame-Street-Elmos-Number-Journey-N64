typedef struct {
    u8 pad0[0xC];
    s32 unk_C;
    u8 pad10[0x8];
    s32 unk_18;
    u8 pad1C[0x8];
    f32 unk_24;
    u8 pad28[0xC];
    u8 *unk_34;
    u8 pad38[0x34];
    f32 unk_6C;
    f32 unk_70;
    u8 pad74[0x30];
    u16 unk_A4;
} Track;

/* advances a byte-coded event stream: value bytes set unk_70/unk_24, durations reload unk_A4 */
void func_800500A8(Track *t) {
    u8 c;
    f32 v;
    s32 hi;

    do {
        t->unk_18 += 0x100;
        if (--t->unk_A4 == 0) {
            c = *t->unk_34++;
            if (c >= 0x80) {
                v = (f32)(c & 0x7F) - 64.0;
                t->unk_70 = v;
                t->unk_24 = v * t->unk_6C;
                c = *t->unk_34++;
                if (c >= 0x80) {
                    hi = (c & 0x7F) << 8;
                    t->unk_A4 = hi;
                    t->unk_A4 = *t->unk_34++ + (hi + 2);
                } else {
                    t->unk_A4 = c + 2;
                }
            } else {
                v = (f32)c - 64.0;
                t->unk_A4 = 1;
                t->unk_70 = v;
                t->unk_24 = v * t->unk_6C;
            }
        }
    } while (t->unk_18 - t->unk_C < 0);
}
