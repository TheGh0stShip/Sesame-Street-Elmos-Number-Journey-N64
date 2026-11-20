#include "common.h"


void func_8003CD80(s32 arg0) {
    if ((u32)D_80128443 < 240) {
        D_80128443 = D_80128443 + 16;
        return;
    }
    D_80128443 = 255;
    osViBlack(1);
    if (D_8011551C == 0) {
        if (D_8012FE68 == 22) {
            func_8001E1E4(18);
            return;
        }
        func_8001E1E4(12);
    }
}
