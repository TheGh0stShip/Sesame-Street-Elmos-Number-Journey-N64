#include "common.h"

void func_80041E10(s32 arg0, s32 arg1) {
    if (((struct gd_4_s32 *)arg0)->v != 0) {
        (*(s32 *)((struct gd_4_s32 *)arg0)->v) = *(s32 *)arg0;
    }
    if ((*(s32 *)arg0) != 0) {
        ((struct gd_4_s32 *)(*(s32 *)arg0))->v = ((struct gd_4_s32 *)arg0)->v;
    }
    if (arg0 == (*(s32 *)arg1)) {
        (*(s32 *)arg1) = *(s32 *)arg0;
    }
    if (arg0 == ((struct gd_4_s32 *)arg1)->v) {
        ((struct gd_4_s32 *)arg1)->v = ((struct gd_4_s32 *)arg0)->v;
    }
    (*(s32 *)arg0) = ((struct gd_4_s32 *)arg0)->v = 0;
    if (((struct gd_14_s32 *)arg0)->v != 0) {
        ((s32 (*)())((struct gd_14_s32 *)arg0)->v)(arg0);
    }
    return;
}
