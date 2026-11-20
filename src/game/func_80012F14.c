#include "common.h"

void func_80012F14(s32 arg0) {
    if ((*(s32 *)arg0) != 0) {
        ((struct gd_4_s32 *)(*(s32 *)arg0))->v = ((struct gd_4_s32 *)arg0)->v;
    }
    if (((struct gd_4_s32 *)arg0)->v != 0) {
        (*(s32 *)((struct gd_4_s32 *)arg0)->v) = *(s32 *)arg0;
    } else {
        D_800A7F74 = *(s32 *)arg0;
    }
    (*(s32 *)arg0) = D_80128548;
    D_80128548 = arg0;
    D_8013C72C--;
}
