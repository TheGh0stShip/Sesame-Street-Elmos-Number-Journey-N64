#include "common.h"

/* Converts a 4x2x2 float matrix to fixed-point 16.16 integer/fraction halves. */




void func_8004B6B8(s32 arg0, s32 arg1) {
    s32 sp0;
    s32 sp4;
    s32 sp8;
    s32 spC;
    struct n64dk_e4 *sp10;
    struct n64dk_e4 *sp14;

    sp10 = (void *)arg1;
    sp14 = (void *)(arg1 + 32);
    sp0 = 0;
loop:
    {
        if (sp0 >= 4) {
            return;
        }
        for (sp4 = 0; sp4 < 2; sp4++) {
            sp8 = (s32)(((f32 (*)[2][2])arg0)[sp0][sp4][0] * 65536.0f);
            spC = (s32)(((f32 (*)[2][2])arg0)[sp0][sp4][1] * 65536.0f);
            (*(s32 *)((s32)sp10++)) = (sp8 & 0xFFFF0000) | ((spC >> 16) & 0xFFFF);
            (*(s32 *)((s32)sp14++)) = ((sp8 << 16) & 0xFFFF0000) | (spC & 0xFFFF);
        }
        sp0++;
        goto loop;
    }
}
