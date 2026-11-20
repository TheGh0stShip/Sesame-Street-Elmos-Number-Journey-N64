#include "common.h"


void func_800327A4(s32 arg0) {
    s32 sp10;
    s32 sp18[26];
    s32 sp80;

    if (D_80076917 != 0) {
        sp10 = func_8000E030();
        func_8000E2D0(1);
        func_8000E264(255, 255, 255, 255);
        func_8000E1EC(70, 20);
        sprintf(((s32)&sp18), ((s32)&D_8007ADA8), ((D_80076916 * 20) + ((s32)&D_8007693C)));
        func_8000E32C(((s32)&sp18));
        sp80 = (s32)D_8013EC7C++;
        (*(s32 *)sp80) = 0x6000000;
        ((struct gd_4_s32 *)sp80)->v = (s32)&D_80075580;
        func_8000E16C(sp10);
    }
}
