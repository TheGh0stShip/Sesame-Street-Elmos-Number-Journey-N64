typedef struct {
    u8 unk_00[0x18A]; s16 unk_18A; s16 unk_18C; u8 unk_18E[0x1A0 - 0x18E];
    u32 buf[0x400]; u8 unk_11A0[0x11C8 - 0x11A0]; s32 bitpos;
} GsmDec;
void func_80060D80(s32, s32);
void func_8006B488(GsmDec *, s16 *, s16 *, s16 *, s16 *, s16 *, s16 *, s32);

#define GETBITS(dst, n)                                              \
    if (left > (n)) {                                                \
        dst = word & ((1 << (n)) - 1);                               \
        word >>= (n);                                                \
        left -= (n);                                                 \
    } else {                                                         \
        pos += 32;                                                   \
        dst = ((1 << left) - 1) & word;                              \
        word = d->buf[(pos >> 5) & 0x3FF];                           \
        if (left != (n)) {                                           \
            dst |= (word & ((1 << ((n) - left)) - 1)) << left;       \
        }                                                            \
        word >>= (n) - left;                                         \
        left += 32 - (n);                                            \
    }

/* reads one GSM 06.10 frame (or a silence/repeat run header) from the bit ring buffer and passes it to the decoder */
s32 func_8006B800(GsmDec *d, s32 out) {
    s16 LARc[8];
    s16 Nc[4];
    s16 Mc[4];
    s16 bc[4];
    s16 xmaxc[4];
    s16 xmc[4][13];
    s32 i;
    s32 left;
    u32 word;
    s32 pos;
    s32 pad0;
    s32 pad1;

    pos = d->bitpos;
    word = d->buf[(pos >> 5) & 0x3FF] >> (pos & 0x1F);
    left = 32 - (pos & 0x1F);
    if (d->unk_18A == 0 && d->unk_18C == 0) {
        GETBITS(i, 1);
        if (i) {
            GETBITS(d->unk_18C, 4);
            d->unk_18C++;
        } else {
            GETBITS(d->unk_18A, 7);
            d->unk_18A++;
        }
    }
    if (d->unk_18C != 0) {
        func_80060D80(out, 0x140);
        d->unk_18C--;
    } else {
        GETBITS(LARc[0], 6);
        GETBITS(LARc[1], 6);
        GETBITS(LARc[2], 5);
        GETBITS(LARc[3], 5);
        GETBITS(LARc[4], 4);
        GETBITS(LARc[5], 4);
        GETBITS(LARc[6], 3);
        GETBITS(LARc[7], 3);
        for (i = 0; i < 4; i++) {
            GETBITS(Nc[i], 7);
            GETBITS(bc[i], 2);
            GETBITS(Mc[i], 2);
            GETBITS(xmaxc[i], 6);
            GETBITS(xmc[i][0], 3);
            GETBITS(xmc[i][1], 3);
            GETBITS(xmc[i][2], 3);
            GETBITS(xmc[i][3], 3);
            GETBITS(xmc[i][4], 3);
            GETBITS(xmc[i][5], 3);
            GETBITS(xmc[i][6], 3);
            GETBITS(xmc[i][7], 3);
            GETBITS(xmc[i][8], 3);
            GETBITS(xmc[i][9], 3);
            GETBITS(xmc[i][10], 3);
            GETBITS(xmc[i][11], 3);
            GETBITS(xmc[i][12], 3);
        }
        d->unk_18A--;
        func_8006B488(d, LARc, Nc, bc, Mc, xmaxc, xmc[0], out);
    }
    d->bitpos = (pos & ~0x1F) - left + 0x20;
    return 0;
}
