#include "common.h"

void func_800464C0(s32 arg0, s32 arg1) {
    if (((struct gd_4_s32 *)arg1)->v != 0) {
        (*(s32 *)((struct gd_4_s32 *)arg1)->v) = *(s32 *)arg1;
    }
    if ((*(s32 *)arg1) != 0) {
        ((struct gd_4_s32 *)(*(s32 *)arg1))->v = ((struct gd_4_s32 *)arg1)->v;
    }
    if ((*(s32 *)arg0) == arg1) {
        (*(s32 *)arg0) = *(s32 *)arg1;
    }
    if (((struct gd_4_s32 *)arg0)->v == arg1) {
        ((struct gd_4_s32 *)arg0)->v = ((struct gd_4_s32 *)arg1)->v;
    }
    (*(s32 *)arg1) = 0;
    ((struct gd_4_s32 *)arg1)->v = 0;
}
