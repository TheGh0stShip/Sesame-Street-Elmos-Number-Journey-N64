#include "common.h"


void func_8003D094(s32 arg0) {
    s32 pad0[10];
    s32 sp38;
    s32 pad1;
    s32 pad2;
    s32 sp44;

    sp44 = 0;
    sp38 = 0;
    ((struct gd_C_s32 *)arg0)->v = ((struct gd_C_s32 *)arg0)->v & -129;
    if (((struct gd_204_s32 *)arg0)->v != 0) {
        func_800375A8(arg0, 1);
        return;
    }
    if ((((struct gd_1FC_s32 *)arg0)->v & 0x80000000) == 0) {
        func_800375A8(arg0, 2);
        return;
    }
    if ((((struct gd_C_s32 *)arg0)->v & 8) != 0) {
        func_800375A8(arg0, 0);
    }
}
