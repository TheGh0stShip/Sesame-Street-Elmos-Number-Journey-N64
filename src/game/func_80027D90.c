#include "common.h"
/* Loop: streams data from D_800A7A50 in 0x4000-byte chunks via osPiStartDma. */



extern s32 D_80118244;

void func_80027D90(s32 arg0) {
    s32 sp20[6];
    s32 sp38;
    s32 sp3C;
    s8 pad0;

    osCreateMesgQueue(((s32)&D_80140F30), ((s32)&D_8010E668), 1);
    while (1) {
        osRecvMesg(((s32)&D_80140F30), 0, 1);
        sp38 = func_80042080(D_800A7A50);
        D_80118244 = sp38;
        switch (D_80118244) {
        case 0:
            func_80059540(((s32)&D_8007A8C0), ((s32)&D_8007A8C4), 216);
        }
        for (; !(D_800A7A50 == 0); D_800A7A50 = D_800A7A50 - sp3C) {
            if ((u32)D_800A7A50 >= 0x4001) {
                sp3C = 0x4000;
            } else {
                sp3C = D_800A7A50;
            }
            osInvalDCache(sp38, sp3C);
            osPiStartDma(((s32)&sp20), 0, 0, D_80106C74, sp38, sp3C, ((s32)&D_80140F30));
            osRecvMesg(((s32)&D_80140F30), 0, 1);
            D_80106C74 = D_80106C74 + sp3C;
            sp38 = sp38 + sp3C;
        }
        D_80106C74 = 0;
    }
}
