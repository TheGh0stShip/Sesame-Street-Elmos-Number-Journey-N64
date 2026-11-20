#include "common.h"


void func_80003B10(void) {
    s32 sp0;
    s32 sp4;

    if (D_80128443 != 0) {
        sp0 = (s32)D_8013EC7C++;
        (*(s32 *)sp0) = 0xB900031D;
        ((struct gd_4_s32 *)sp0)->v = 0x504240;
        sp0 = (s32)D_8013EC7C++;
        (*(s32 *)sp0) = 0xFCFFFFFF;
        ((struct gd_4_s32 *)sp0)->v = 0xFFFEFB7D;
        sp0 = (s32)D_8013EC7C++;
        (*(s32 *)sp0) = 0xBA000E02;
        ((struct gd_4_s32 *)sp0)->v = 0;
        sp0 = (s32)D_8013EC7C++;
        (*(s32 *)sp0) = 0xBB000000;
        ((struct gd_4_s32 *)sp0)->v = 0x80008000;
        sp0 = (s32)D_8013EC7C++;
        (*(s32 *)sp0) = 0xFB000000;
        ((struct gd_4_s32 *)sp0)->v = ((((*(u8 (*))&D_80128440) << 24) | (D_80128441 << 16)) | (D_80128442 << 8)) | D_80128443;
        sp0 = (s32)D_8013EC7C++;
        (*(s32 *)sp0) = 0xE45003C0;
        ((struct gd_4_s32 *)sp0)->v = 0;
        sp4 = (s32)D_8013EC7C++;
        (*(s32 *)sp4) = 0xB4000000;
        ((struct gd_4_s32 *)sp4)->v = 0;
        sp4 = (s32)D_8013EC7C++;
        (*(s32 *)sp4) = 0xB3000000;
        ((struct gd_4_s32 *)sp4)->v = 0x4000400;
    }
    sp4 = (s32)D_8013EC7C++;
    (*(s32 *)sp4) = 0xE7000000;
    ((struct gd_4_s32 *)sp4)->v = 0;
    sp4 = (s32)D_8013EC7C++;
    (*(s32 *)sp4) = 0xBB000001;
    ((struct gd_4_s32 *)sp4)->v = 0x80008000;
}
