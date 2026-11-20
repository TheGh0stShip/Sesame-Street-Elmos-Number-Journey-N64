#include "common.h"


void func_80040704(s32 arg0) {
    s32 sp10;
    s32 sp14;

    sp10 = arg0;
    sp14 = (s32)D_8013EC7C++;
    (*(s32 *)sp14) = 0xE7000000;
    ((struct gd_4_s32 *)sp14)->v = 0;
    sp14 = (s32)D_8013EC7C++;
    (*(s32 *)sp14) = 0xFC127E24;
    ((struct gd_4_s32 *)sp14)->v = -3079;
    sp14 = (s32)D_8013EC7C++;
    (*(s32 *)sp14) = 0xBC000C06;
    ((struct gd_4_s32 *)sp14)->v = osVirtualToPhysical(((struct gd_C_s32 *)((struct gd_158_s32 *)sp10)->v)->v);
    sp14 = (s32)D_8013EC7C++;
    (*(s32 *)sp14) = 0xBC001006;
    ((struct gd_4_s32 *)sp14)->v = osVirtualToPhysical(((struct gd_1C_s32 *)((struct gd_158_s32 *)sp10)->v)->v);
    sp14 = (s32)D_8013EC7C++;
    (*(s32 *)sp14) = 0x1020040;
    ((struct n64dk_v4 *)sp14)->v = (void *)&((struct n64dk_m58_40 *)sp10)->m[D_800717E0];
    sp14 = (s32)D_8013EC7C++;
    (*(s32 *)sp14) = 0x6000000;
    ((struct gd_4_s32 *)sp14)->v = ((struct gd_8_s32 *)((struct gd_158_s32 *)sp10)->v)->v;
}
