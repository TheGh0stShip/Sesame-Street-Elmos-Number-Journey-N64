#include "common.h"


void func_8002E154(s32 arg0) {
    s32 sp10;
    s32 sp14;
    s32 sp18;

    sp10 = arg0;
    sp14 = sp10 + 376;
    sp18 = (s32)D_8013EC7C++;
    (*(s32 *)sp18) = 0xE7000000;
    ((struct gd_4_s32 *)sp18)->v = 0;
    sp18 = (s32)D_8013EC7C++;
    (*(s32 *)sp18) = 0xFC127E24;
    ((struct gd_4_s32 *)sp18)->v = -3079;
    sp18 = (s32)D_8013EC7C++;
    (*(s32 *)sp18) = 0x1020040;
    ((struct gd_4_s32 *)sp18)->v = ((struct gd_14_s32 *)sp14)->v;
    sp18 = (s32)D_8013EC7C++;
    (*(s32 *)sp18) = 0xBC000C06;
    ((struct gd_4_s32 *)sp18)->v = osVirtualToPhysical(((struct gd_C_s32 *)sp14)->v);
    sp18 = (s32)D_8013EC7C++;
    (*(s32 *)sp18) = 0xBC001006;
    ((struct gd_4_s32 *)sp18)->v = osVirtualToPhysical(((struct gd_18_s32 *)sp14)->v);
    sp18 = (s32)D_8013EC7C++;
    (*(s32 *)sp18) = 0x6000000;
    ((struct gd_4_s32 *)sp18)->v = ((struct gd_10_s32 *)sp14)->v;
    sp18 = (s32)D_8013EC7C++;
    (*(s32 *)sp18) = 0x6000000;
    ((struct gd_4_s32 *)sp18)->v = (s32)&D_80075580;
}
