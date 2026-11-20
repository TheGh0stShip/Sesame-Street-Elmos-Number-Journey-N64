typedef struct {
    u8 pad[0xCE];
    u8 unk_CE;
    u8 unk_CF;
    u8 unk_D0;
    s8 unk_D1;
    u8 pad2[6];
    u8 unk_D8;
} Blink;

/* counts down a timer, toggling unk_D1 between 0 and unk_D8; returns unk_D1 */
f32 func_8004FF28(Blink *b) {
    if (--b->unk_D0 == 0) {
        if (b->unk_D1 == 0) {
            b->unk_D1 = b->unk_D8;
            b->unk_D0 = b->unk_CE;
        } else {
            b->unk_D1 = 0;
            b->unk_D0 = b->unk_CF;
        }
    }
    return b->unk_D1;
}
