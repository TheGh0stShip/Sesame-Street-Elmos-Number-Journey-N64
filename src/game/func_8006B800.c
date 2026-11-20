#include "common.h"
#include "codec/stream.h"

void func_8006B488(stream_t *, s16 *, s16 *, s16 *, s16 *, s16 *, s16 *, s32);

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
s32 func_8006B800(stream_t *d, s32 out) {
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
    if (d->frame_run == 0 && d->silence_run == 0) {
        GETBITS(i, 1);
        if (i) {
            GETBITS(d->silence_run, 4);
            d->silence_run++;
        } else {
            GETBITS(d->frame_run, 7);
            d->frame_run++;
        }
    }
    if (d->silence_run != 0) {
        bzero(out, 0x140);
        d->silence_run--;
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
        d->frame_run--;
        func_8006B488(d, LARc, Nc, bc, Mc, xmaxc, xmc[0], out);
    }
    d->bitpos = (pos & ~0x1F) - left + 0x20;
    return 0;
}
