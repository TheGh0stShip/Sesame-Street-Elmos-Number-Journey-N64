#include "common.h"
 extern u64 D_80083028, D_80083020, D_80083030, D_80083040;
extern s32 D_80109D58, D_80109D5C, D_80107D14, D_80133FB0, D_801153F8;
extern s32 D_800717F4, D_800717F8, D_80071800, D_800717FC, D_80071810, D_80071804, D_8007180C, D_800717F0;
/* resets timers from the current time divided by 46875 and clears state */
void func_8000D3C0(void) {
    D_80083030 = D_80083020 = D_80083028 = D_80083038 = __udivdi3(osGetTime(), 46875);
    D_80083040 = 0;
    D_80109D58 = 0;
    D_80109D5C = 0;
    D_80107D14 = 0;
    D_80133FB0 = 0;
    D_801153F8 = 0;
    D_800717F4 = 0;
    D_800717F8 = 0;
    D_80071800 = 0;
    D_800717FC = 0;
    D_80071810 = 0;
    D_80071804 = 0;
    D_8007180C = 0;
    D_800717F0 = 1;
}
