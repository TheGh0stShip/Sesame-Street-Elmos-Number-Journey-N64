#include "common.h"


void func_80017788(s32 arg0, s32 arg1, s32 arg2) {
    s32 sp20[6];
    s32 sp38;
    s32 sp3C;

    while (!(arg2 == 0)) {
        if (arg2 >= 0x4001) {
            sp3C = 0x4000;
        } else {
            sp3C = arg2;
        }
        osInvalDCache(arg1, sp3C);
        osPiStartDma(((s32)&sp20), 0, 0, arg0, arg1, sp3C, ((s32)&D_80107CF8));
        osRecvMesg(((s32)&D_80107CF8), ((s32)&sp38), 1);
        arg0 = arg0 + sp3C;
        arg1 = arg1 + sp3C;
        arg2 = arg2 - sp3C;
    }
}
