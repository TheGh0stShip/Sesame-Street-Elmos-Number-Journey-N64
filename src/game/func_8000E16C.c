#include "common.h"

s32 func_8005F16C();

void func_8000E16C(s32 arg0) {
    s32 sp10;

    sp10 = arg0;
    func_8005F16C(((s32)&sp10));
    arg0 = sp10 - 8;
}
