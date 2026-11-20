#include "common.h"


void func_8003669C(s32 arg0) {
    s32 sp10;

    sp10 = arg0;
    if (((struct gd_EC_s32 *)sp10)->v != 0) {
        func_80042300(((struct gd_EC_s32 *)sp10)->v);
    }
    if (((struct gd_F0_s32 *)sp10)->v != 0) {
        func_80042300(((struct gd_F0_s32 *)sp10)->v);
    }
    func_80042300(sp10);
}
