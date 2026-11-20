#include "common.h"


s32 func_8004D340(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    register s32 reg_s0;
    f32 sp10;
    f32 sp14;
    f32 sp18;

    reg_s0 = func_80042080(156);
    func_80069C70(reg_s0, 0, 156);
    (*(s32 *)reg_s0) = arg0;
    sp10 = *(f32 *)arg2;
    sp14 = ((struct gd_4_f32 *)arg2)->v;
    sp18 = ((struct gd_8_f32 *)arg2)->v;
    ((struct gd_4_s32 *)reg_s0)->v = 0;
    ((struct gd_6C_s32 *)reg_s0)->v = ((struct gd_30_s32 *)arg1)->v;
    ((struct gd_14_f32 *)reg_s0)->v = (f32)arg3;
    ((struct gd_40_s32 *)reg_s0)->v = 0;
    ((struct gd_44_s32 *)reg_s0)->v = -1;
    ((struct gd_70_s32 *)reg_s0)->v = 0;
    ((struct gd_74_s32 *)reg_s0)->v = 0;
    ((struct gd_78_s32 *)reg_s0)->v = reg_s0 + 124;
    ((struct gd_7C_s32 *)reg_s0)->v = 1;
    ((struct gd_80_s32 *)reg_s0)->v = reg_s0;
    ((struct gd_84_s32 *)reg_s0)->v = 0;
    ((struct gd_98_f32 *)reg_s0)->v = ((struct gd_14_f32 *)reg_s0)->v;
    ((struct gd_8C_f32 *)reg_s0)->v = sp10;
    ((struct gd_90_f32 *)reg_s0)->v = -sp18;
    ((struct gd_94_f32 *)reg_s0)->v = sp14;
    func_80046250(((struct gd_6C_s32 *)reg_s0)->v, (reg_s0 + 112), 2, 0);
    switch (D_8012F154) {
    case 5:
        D_80107CD0 = 100;
        D_800A913C = 250;
        D_801153F4 = 12;
        D_800A7A54 = 36;
        break;
    default:
        D_80107CD0 = 20;
        D_800A913C = 70;
        D_801153F4 = 2;
        D_800A7A54 = 12;
        break;
    }
    D_8013ED40 = (f32)D_80107CD0;
    return reg_s0;
}
