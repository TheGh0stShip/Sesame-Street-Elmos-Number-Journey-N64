#include "common.h"

void func_800375A8(s32 arg0, s32 arg1) {
    ((struct gd_1C8_s32 *)arg0)->v = arg1;
    ((struct gd_1CC_s32 *)arg0)->v = 0;
    ((struct gd_224_s16 *)arg0)->v = 0;
    ((s32 (*)())(*(s32 *)((arg1 * 4) + ((s32)&D_80076BA0))))(arg0);
}
