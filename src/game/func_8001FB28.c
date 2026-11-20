/* keeps its own unprototyped gu*F declarations: called with s32 arguments where the SDK prototype takes float (a prototype converts them: code changes) */
#define GAME_PRIVATE_SDK
#include "common.h"

s32 guMtxF2L();
s32 guRotateF(s32, s32, s32, s32, f32);
s32 guScaleF();

void func_8001FB28(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, f32 arg5, s32 arg6, s32 arg7, s32 arg8) {
    s32 sp18[16];
    s32 sp58[16];
    s32 sp98[16];
    s32 spD8[16];

    func_8001E5F0(((s32)&sp18), arg1);
    guRotateF(((s32)&sp98), arg2, arg3, arg4, arg5);
    guMtxCatF(((s32)&sp98), ((s32)&sp18), ((s32)&spD8));
    guScaleF(((s32)&sp58), arg6, arg7, arg8);
    guMtxCatF(((s32)&sp58), ((s32)&spD8), ((s32)&spD8));
    guMtxF2L(((s32)&spD8), arg0);
}
