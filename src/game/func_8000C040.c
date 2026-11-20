#include "common.h"

extern s32 D_80081B10;

void func_8000C040(s32 arg0) {
    s32 pad0[16];
    s8 pad1;

    osInitialize();
    osCreateThread(((s32)&D_8007D960), 1, ((s32)&D_8000C0A8), arg0, ((s32)&D_80081B10), 10);
    osStartThread(((s32)&D_8007D960));
}
