#include "common.h"

extern u16 D_80107CBA;

void func_80038210(s32 arg0) {
    if (((D_80107CBA & 0x8000) & 0xFFFF) != 0) {
        D_8012FE68 = 1;
        func_800375A8(arg0, 35);
    }
}
