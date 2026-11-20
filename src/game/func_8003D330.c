#include "common.h"


void func_8003D330(s32 arg0) {
    if (((struct gd_204_s32 *)arg0)->v != 0) {
        func_800375A8(arg0, 1);
        return;
    }
    if ((((struct gd_1FC_s32 *)arg0)->v & 0x80000000) == 0) {
        func_800375A8(arg0, 2);
        return;
    }
    if ((((struct gd_C_s32 *)arg0)->v & 8) != 0) {
        func_800375A8(arg0, 0);
    }
}
