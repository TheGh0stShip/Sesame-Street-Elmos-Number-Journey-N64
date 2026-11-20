#include "common.h"

extern void func_8006B488(s32, s16 *, s16 *, s16 *, s16 *, s16 *, s16 (*)[13], s32);
/* unpacks a bit-packed frame header (5 bytes) and 4 sub-blocks (7 bytes each), then decodes */
s32 func_8006B570(s32 arg0, u8 *p, s32 arg2) {
    s16 hdr[8];
    s16 a[4];
    s16 c[4];
    s16 b[4];
    s16 e[4];
    s16 sub[4][13];
    s32 i;

    hdr[0] = ((p[0] & 0xF) << 2) | ((p[1] >> 6) & 3);
    hdr[1] = p[1] & 0x3F;
    hdr[2] = (p[2] >> 3) & 0x1F;
    hdr[3] = ((p[2] & 7) << 2) | ((p[3] >> 6) & 3);
    hdr[4] = (p[3] >> 2) & 0xF;
    hdr[5] = ((p[3] & 3) << 2) | ((p[4] >> 6) & 3);
    hdr[6] = (p[4] >> 3) & 7;
    hdr[7] = p[4] & 7;
    for (i = 0; i < 4; i++) {
        a[i] = (p[i * 7 + 5] >> 1) & 0x7F;
        b[i] = ((p[i * 7 + 5] & 1) << 1) | ((p[i * 7 + 6] >> 7) & 1);
        c[i] = (p[i * 7 + 6] >> 5) & 3;
        e[i] = ((p[i * 7 + 6] & 0x1F) << 1) | ((p[i * 7 + 7] >> 7) & 1);
        sub[i][0] = (p[i * 7 + 7] >> 4) & 7;
        sub[i][1] = (p[i * 7 + 7] >> 1) & 7;
        sub[i][2] = ((p[i * 7 + 7] & 1) << 2) | ((p[i * 7 + 8] >> 6) & 3);
        sub[i][3] = (p[i * 7 + 8] >> 3) & 7;
        sub[i][4] = p[i * 7 + 8] & 7;
        sub[i][5] = (p[i * 7 + 9] >> 5) & 7;
        sub[i][6] = (p[i * 7 + 9] >> 2) & 7;
        sub[i][7] = ((p[i * 7 + 9] & 3) << 1) | ((p[i * 7 + 10] >> 7) & 1);
        sub[i][8] = (p[i * 7 + 10] >> 4) & 7;
        sub[i][9] = (p[i * 7 + 10] >> 1) & 7;
        sub[i][10] = ((p[i * 7 + 10] & 1) << 2) | ((p[i * 7 + 11] >> 6) & 3);
        sub[i][11] = (p[i * 7 + 11] >> 3) & 7;
        sub[i][12] = p[i * 7 + 11] & 7;
    }
    func_8006B488(arg0, hdr, a, b, c, e, sub, arg2);
    return 0;
}
