#include "common.h"


void func_80042D10(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    s32 sp10[12];
    f32 sp40;
    f32 sp44;
    f32 sp48;

    (*(s32 *)arg0) = *(s32 *)arg1;
    ((struct gd_4_s32 *)arg0)->v = ((struct gd_4_s32 *)arg1)->v;
    ((struct gd_8_s32 *)arg0)->v = ((struct gd_8_s32 *)arg1)->v;
    ((struct gd_C_s32 *)arg0)->v = ((struct gd_C_s32 *)arg1)->v;
    func_80044018(((s32)&sp10), arg3, arg4);
    func_800441EC((arg0 + 16), (arg1 + 16), ((s32)&sp10));
    ((struct gd_1C_f32 *)arg0)->v = (((((struct gd_10_f32 *)arg0)->v * (*(f32 *)arg5)) + (((struct gd_14_f32 *)arg0)->v * ((struct gd_4_f32 *)arg5)->v)) + (((struct gd_18_f32 *)arg0)->v * ((struct gd_8_f32 *)arg5)->v)) + ((struct gd_1C_f32 *)arg1)->v;
    sp40 = *(f32 *)arg5;
    sp44 = ((struct gd_4_f32 *)arg5)->v;
    sp48 = ((struct gd_8_f32 *)arg5)->v;
    func_800441EC((arg0 + 32), (arg1 + 32), ((s32)&sp10));
    func_800441EC((arg0 + 44), (arg1 + 44), ((s32)&sp10));
    func_800441EC((arg0 + 56), (arg1 + 56), ((s32)&sp10));
    func_800441EC((arg0 + 68), (arg1 + 68), ((s32)&sp10));
}
