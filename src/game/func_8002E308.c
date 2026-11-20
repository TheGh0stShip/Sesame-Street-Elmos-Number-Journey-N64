#include "common.h"

extern s32 D_80106C84;

void func_8002E308(s32 arg0) {
    s32 sp10;

    sp10 = arg0;
    func_80042300(((struct gd_F0_s32 *)sp10)->v);
    D_80106C84 = 0;
    func_80042300(sp10);
    return;
}
