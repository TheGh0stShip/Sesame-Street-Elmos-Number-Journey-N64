#include "common.h"

void func_80037450(s32 arg0, s32 arg1) {
    ((struct gd_1D4_s32 *)arg0)->v = arg1;
    ((struct gd_C_s32 *)arg0)->v = ((struct gd_C_s32 *)arg0)->v & -9;
    ((struct gd_224_s16 *)arg0)->v = 0;
}
