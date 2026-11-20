#include "common.h"


void func_8003ADE4(s32 arg0) {
    func_80037450(arg0, 21);
    ((struct gd_C_s32 *)arg0)->v = ((struct gd_C_s32 *)arg0)->v & -129;
    ((struct gd_200_s32 *)arg0)->v = ((struct gd_200_s32 *)arg0)->v | 1;
    ((struct gd_200_s32 *)arg0)->v = ((struct gd_200_s32 *)arg0)->v | 2;
    ((struct gd_200_s32 *)arg0)->v = ((struct gd_200_s32 *)arg0)->v | 4;
    (*(f32 *)(arg0 + 536)) = *(f32 *)(arg0 + 36);
    ((struct gd_4_f32 *)(arg0 + 536))->v = ((struct gd_4_f32 *)(arg0 + 36))->v;
    ((struct gd_8_f32 *)(arg0 + 536))->v = ((struct gd_8_f32 *)(arg0 + 36))->v;
    if (D_800A7F2C == 0) {
        D_800A7F2C = func_8004E5A0(31);
        D_8012F138 = 0;
        ((s32 (*)())func_8004E990)(D_800A7F2C, D_8012F138);
        ((struct gd_1EF_s8 *)arg0)->v = 0;
        ((struct gd_1EE_s8 *)arg0)->v = 0;
    }
}
