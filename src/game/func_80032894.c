#include "common.h"


void func_80032894(s32 arg0) {
    s32 sp0;

    sp0 = (s32)D_8013EC7C++;
    (*(s32 *)sp0) = 0xBC00000E;
    ((struct gd_4_s32 *)sp0)->v = D_800A7F00;
    sp0 = (s32)D_8013EC7C++;
    (*(s32 *)sp0) = 0x1030040;
    ((struct gd_4_s32 *)sp0)->v = D_8013EF28;
}
