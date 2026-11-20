#include "common.h"


void func_8003670C(s32 arg0) {
    s32 sp10;
    s32 sp14;

    sp10 = arg0;
    sp14 = (s32)D_8013EC7C++;
    (*(s32 *)sp14) = 0xE7000000;
    ((struct gd_4_s32 *)sp14)->v = 0;
    sp14 = (s32)D_8013EC7C++;
    (*(s32 *)sp14) = 0xBC000C06;
    ((struct gd_4_s32 *)sp14)->v = osVirtualToPhysical(((struct gd_C_s32 *)((struct gd_1E8_s32 *)sp10)->v)->v);
    if (((struct gd_1D8_s32 *)sp10)->v == 0) {
        sp14 = (s32)D_8013EC7C++;
        (*(s32 *)sp14) = 0xBC001006;
        ((struct gd_4_s32 *)sp14)->v = osVirtualToPhysical(((struct gd_1C_s32 *)((struct gd_1E8_s32 *)sp10)->v)->v);
    } else {
        switch (D_8012F154) {
        case 9:
        case 10:
            if (((struct gd_1D8_s32 *)sp10)->v >= 2) {
                sp14 = (s32)D_8013EC7C++;
                (*(s32 *)sp14) = 0xBC001006;
                ((struct gd_4_s32 *)sp14)->v = osVirtualToPhysical(((s32)&D_802AF280));
            } else {
                sp14 = (s32)D_8013EC7C++;
                (*(s32 *)sp14) = 0xBC001006;
                ((struct gd_4_s32 *)sp14)->v = osVirtualToPhysical(((s32)&D_802AF4A0));
            }
            break;
        case 7:
        case 8:
            if (((struct gd_1D8_s32 *)sp10)->v >= 2) {
                sp14 = (s32)D_8013EC7C++;
                (*(s32 *)sp14) = 0xBC001006;
                ((struct gd_4_s32 *)sp14)->v = osVirtualToPhysical(((s32)&D_802AF6C0));
            } else {
                sp14 = (s32)D_8013EC7C++;
                (*(s32 *)sp14) = 0xBC001006;
                ((struct gd_4_s32 *)sp14)->v = osVirtualToPhysical(((s32)&D_802AF8E0));
            }
            break;
            break;
        case 5:
        case 6:
            if (((struct gd_1D8_s32 *)sp10)->v >= 2) {
                sp14 = (s32)D_8013EC7C++;
                (*(s32 *)sp14) = 0xBC001006;
                ((struct gd_4_s32 *)sp14)->v = osVirtualToPhysical(((s32)&D_802AFB00));
            } else {
                sp14 = (s32)D_8013EC7C++;
                (*(s32 *)sp14) = 0xBC001006;
                ((struct gd_4_s32 *)sp14)->v = osVirtualToPhysical(((s32)&D_802AFD20));
            }
            break;
            break;
        }
    }
    sp14 = (s32)D_8013EC7C++;
    (*(s32 *)sp14) = 0x1020040;
    ((struct n64dk_v4 *)sp14)->v = (void *)&((struct n64dk_mF8_40 *)sp10)->m[D_800717E0];
    sp14 = (s32)D_8013EC7C++;
    (*(s32 *)sp14) = 0x6000000;
    ((struct gd_4_s32 *)sp14)->v = ((struct gd_8_s32 *)((struct gd_1E8_s32 *)sp10)->v)->v;
}
