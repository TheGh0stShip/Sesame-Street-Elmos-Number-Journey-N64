#include "common.h"

s32 func_8005CF90();
s32 func_8005E4F8();

void func_8000E32C(s32 arg0) {
    s32 sp18;
    s32 sp1C;
    s32 sp20;

    sp18 = (s32)&D_80072D88;
    ((struct gd_4_s16 *)sp18)->v = (D_80072E9C * 8) + 8;
    ((struct gd_6_s16 *)sp18)->v = (D_80072EA0 * 16) + 8;
    func_8000DC80(sp18, arg0, ((s32)&D_80072D7C), D_80072E9C, D_80072EA0);
    func_8005D068(sp18, D_80072E94, D_80072E98);
    func_8005CF90(sp18, (*(u8 (*))&D_80072E90), (*(u8 (*))&D_80072E91), (*(u8 (*))&D_80072E92), (*(u8 (*))&D_80072E93));
    spScale(sp18, ((f32)D_80072EA8), ((f32)D_80072EB0));
    sp1C = func_8005E4F8(sp18);
    sp20 = (s32)D_8013EC7C++;
    (*(s32 *)sp20) = 0x6000000;
    ((struct gd_4_s32 *)sp20)->v = sp1C;
}
