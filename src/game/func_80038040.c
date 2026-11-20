#include "common.h"


void func_80038040(s32 arg0) {
    func_800374D0(arg0);
    func_80037450(arg0, 0);
    ((struct gd_C_s32 *)arg0)->v = ((struct gd_C_s32 *)arg0)->v & -129;
    ((struct gd_200_s32 *)arg0)->v = ((struct gd_200_s32 *)arg0)->v & -2;
    ((struct gd_200_s32 *)arg0)->v = ((struct gd_200_s32 *)arg0)->v & -3;
    ((struct gd_200_s32 *)arg0)->v = ((struct gd_200_s32 *)arg0)->v & -5;
    ((struct gd_226_s16 *)arg0)->v = 8;
}
