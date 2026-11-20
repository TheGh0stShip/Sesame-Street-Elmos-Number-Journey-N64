#include "common.h"

#define DIV(a,b) ((a) / (b))

extern f64 D_80109D58;

/* Per-frame timing: measures the frame time, frame rate and accumulated lag counters. */
void func_8000D53C(void) {
    u64 step;
    s32 prev;
    s32 over;
    s32 rest;
    s32 half;

    if (D_800717F0 == 0) {
        func_8000D3C0();
    }
    D_80083038 = D_80083030;
    D_80083030 = DIV(osGetTime(), 46875);
    D_80083040 = D_80083030 - D_80083038;
    D_8012F160 = D_80083040;
    if (D_80083040 != 0) {
        D_80109D58 = 1.0 / (D_8012F160 * 0.001);
    } else {
        D_80109D58 = 0.0;
    }
    if (D_8007180C != 0) {
        D_80071800 = 0;
        D_800717FC = 0;
        D_800717F4 = 0;
        D_800717F8 = 0;
    }
    if (D_8007180C != 0 || D_80071804 != 0 || D_80083040 > 70) {
        step = 33;
        over = D_80083040 - 33;
        D_80083020 += over;
        D_80083028 += over;
        D_80071808 = 1;
    } else {
        step = D_80083040;
        D_80071808 = 0;
    }
    D_800717F8 = D_800717FC;
    D_800717F4 = D_80071800;
    if (D_800717F8 != 0) {
        D_80083020 += step;
        D_80083028 += step;
    }
    if (D_8007180C != 0) {
        D_80071800 = 1;
        D_8007180C = 0;
    }
    prev = D_801153F8;
    D_801153F8 = D_80083030 - D_80083020;
    D_80106C8C = D_801153F8 - prev;
    if (D_800717F4 && (D_800717F8 == 0)) D_80083028 += D_80106C8C;
    else if (D_80071810 != 0) {
        half = DIV(step, 2);
        rest = step - half;
        D_80083028 += rest;
    }
    prev = D_80133FB0;
    D_80133FB0 = D_80083030 - D_80083028;
    D_80107D14 = D_80133FB0 - prev;
}
