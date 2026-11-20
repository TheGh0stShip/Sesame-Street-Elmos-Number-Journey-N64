#include "common.h"

void func_80042300(s32 arg0) {
    s32 sp0;
    s32 sp4;
    s32 sp8;
    s32 spC;

    switch (arg0 & 15) {
    case 0:
        sp0 = arg0 - 16;
        if ((((struct gd_C_s32 *)sp0)->v & 15) == 0) {
            return;
        }
        spC = ((struct gd_8_s32 *)sp0)->v;
        ((struct gd_8_s32 *)sp0)->v = D_80115374[spC].m0;
        D_80115374[spC].m0 = sp0;
        ((struct gd_C_s32 *)sp0)->v = 0;
        if (((struct gd_8_s32 *)sp0)->v != 0) {
            ((struct gd_C_s32 *)((struct gd_8_s32 *)sp0)->v)->v = sp0;
        }
        sp4 = *(s32 *)sp0;
        if (sp4 != 0) {
            if ((((struct gd_C_s32 *)sp4)->v & 15) == 0) {
                ((struct gd_8_s32 *)((struct gd_C_s32 *)sp4)->v)->v = ((struct gd_8_s32 *)sp4)->v;
                if (((struct gd_8_s32 *)sp4)->v != 0) {
                    ((struct gd_C_s32 *)((struct gd_8_s32 *)sp4)->v)->v = ((struct gd_C_s32 *)sp4)->v;
                }
                (*(s32 *)sp0) = *(s32 *)sp4;
                if ((*(s32 *)sp4) != 0) {
                    ((struct gd_4_s32 *)(*(s32 *)sp4))->v = sp0;
                }
            }
        }
        sp8 = ((struct gd_4_s32 *)sp0)->v;
        if (sp8 != 0) {
            if ((((struct gd_C_s32 *)sp8)->v & 15) == 0) {
                D_80115374[spC].m0 = ((struct gd_8_s32 *)sp0)->v;
                ((struct gd_C_s32 *)((struct gd_8_s32 *)sp0)->v)->v = 0;
                (*(s32 *)sp8) = *(s32 *)sp0;
                if ((*(s32 *)sp0) != 0) {
                    ((struct gd_4_s32 *)(*(s32 *)sp0))->v = sp8;
                }
            }
        }
        return;
    }
}
