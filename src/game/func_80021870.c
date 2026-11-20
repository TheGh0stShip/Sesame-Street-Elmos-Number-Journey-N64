#include "common.h"

void func_80021870(s32 arg0) {
    s8 pad0;

    ((struct gd_8_s16 *)arg0)->v = 2;
    ((struct gd_A_s16 *)arg0)->v = 0;
    ((struct gd_C_s32 *)arg0)->v = 0;
    ((struct gd_10_s32 *)arg0)->v = (s32)&D_800218EC;
    ((struct gd_14_s32 *)arg0)->v = (s32)&D_800219DC;
    ((struct gd_18_s32 *)arg0)->v = 0;
    ((struct gd_1C_s32 *)arg0)->v = 0;
    ((struct gd_20_s32 *)arg0)->v = 0;
    ((struct gd_24_s32 *)arg0)->v = 0;
}
