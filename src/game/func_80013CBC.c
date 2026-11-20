#include "common.h"

s32 func_80013CBC(s32 arg0, s32 arg1, s32 arg2) {
    s32 sp0;
    s32 sp4;
    s32 sp8;
    s32 spC;

    sp8 = ((struct gd_C_s32 *)arg1)->v;
    spC = ((struct gd_30_s32 *)arg0)->v;
    sp0 = ((u32)spC <= (u32)((struct gd_34_s32 *)arg0)->v) ? (((struct gd_34_s32 *)arg0)->v - spC) : (((struct gd_2C_s32 *)arg0)->v - spC);
    if ((u32)sp0 > (u32)((struct gd_10_s32 *)arg1)->v) {
        sp0 = ((struct gd_10_s32 *)arg1)->v;
    }
    if (sp0 != 0) {
        if (arg2 == -5) {
            arg2 = 0;
        }
    }
    ((struct gd_10_s32 *)arg1)->v = ((struct gd_10_s32 *)arg1)->v - sp0;
    ((struct gd_14_s32 *)arg1)->v = ((struct gd_14_s32 *)arg1)->v + sp0;
    sp4 = 0;
    while (!((u32)sp4 >= (u32)sp0)) {
        (*(s8 *)(sp8++)) = *(u8 *)(spC++);
        sp4++;
    }
    if (spC == ((struct gd_2C_s32 *)arg0)->v) {
        spC = ((struct gd_28_s32 *)arg0)->v;
        if (((struct gd_34_s32 *)arg0)->v == ((struct gd_2C_s32 *)arg0)->v) {
            ((struct gd_34_s32 *)arg0)->v = ((struct gd_28_s32 *)arg0)->v;
        }
        sp0 = ((struct gd_34_s32 *)arg0)->v - spC;
        if ((u32)sp0 > (u32)((struct gd_10_s32 *)arg1)->v) {
            sp0 = ((struct gd_10_s32 *)arg1)->v;
        }
        if (sp0 != 0) {
            if (arg2 == -5) {
                arg2 = 0;
            }
        }
        ((struct gd_10_s32 *)arg1)->v = ((struct gd_10_s32 *)arg1)->v - sp0;
        ((struct gd_14_s32 *)arg1)->v = ((struct gd_14_s32 *)arg1)->v + sp0;
        sp4 = 0;
        while (!((u32)sp4 >= (u32)sp0)) {
            (*(s8 *)(sp8++)) = *(u8 *)(spC++);
            sp4++;
        }
    }
    ((struct gd_C_s32 *)arg1)->v = sp8;
    ((struct gd_30_s32 *)arg0)->v = spC;
    return arg2;
}
