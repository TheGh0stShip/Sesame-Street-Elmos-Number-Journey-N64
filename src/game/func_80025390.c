#include "common.h"

void func_80025390(s32 arg0) {
    ((struct gd_2_s8 *)arg0)->v = ((struct gd_2_u8 *)arg0)->v + ((struct gd_3_u8 *)arg0)->v;
    if (((struct gd_2_s8 *)arg0)->v >= 2) {
        ((struct gd_3_s8 *)arg0)->v = ((struct gd_3_u8 *)arg0)->v - 2;
    } else {
        if (((struct gd_2_s8 *)arg0)->v < -1) {
            ((struct gd_3_s8 *)arg0)->v = ((struct gd_3_u8 *)arg0)->v + 2;
        } else {
            if (((struct gd_3_s8 *)arg0)->v >= 2) {
                ((struct gd_3_s8 *)arg0)->v = ((struct gd_3_u8 *)arg0)->v - 2;
            } else {
                if (((struct gd_3_s8 *)arg0)->v < -1) {
                    ((struct gd_3_s8 *)arg0)->v = ((struct gd_3_u8 *)arg0)->v + 2;
                } else {
                    ((struct gd_2_s16 *)arg0)->v = 0;
                }
            }
        }
    }
    D_800A93A0 = *(u8 *)arg0;
    D_800A93A1 = ((struct gd_1_u8 *)arg0)->v;
    D_800A93A2 = ((struct gd_2_u8 *)arg0)->v;
    D_800A93A3 = ((struct gd_3_u8 *)arg0)->v;
}
