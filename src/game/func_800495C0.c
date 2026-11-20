#include "common.h"


void func_800495C0(s32 arg0) {
    s32 sp10;

    D_800A0528 = arg0;
    D_800A0524 = func_80042080((D_800A0528 * 16));
    D_800A0520 = D_800A0524;
    sp10 = 0;
    while (!((u32)sp10 >= (u32)(D_800A0528 - 1))) {
        ((struct n64dk_v0 *)((sp10 * 16) + D_800A0524))->v = (void *)&((struct n64dk_m10_10 *)D_800A0524)->m[sp10];
        sp10++;
    }
    (*(s32 *)((sp10 * 16) + D_800A0524)) = 0;
    return;
}
