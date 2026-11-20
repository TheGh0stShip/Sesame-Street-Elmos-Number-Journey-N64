#include "common.h"

s32 func_8005CFB0();
s32 func_8005D050();

void func_8000E2D0(s32 arg0) {
    if (arg0 != 0) {
        func_8005D050(((s32)&D_80072D88), 3);
        return;
    }
    func_8005CFB0(((s32)&D_80072D88), 3);
}
