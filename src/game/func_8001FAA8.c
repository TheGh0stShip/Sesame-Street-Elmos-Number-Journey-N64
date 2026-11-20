/* keeps its own unprototyped gu*F declarations: called with s32 arguments where the SDK prototype takes float (a prototype converts them: code changes) */
#define GAME_PRIVATE_SDK
#include "common.h"

s32 guMtxF2L();
s32 guScaleF();

void func_8001FAA8(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    s32 sp10[16];
    s32 sp50[16];
    s32 sp90[16];

    func_8001E5F0(((s32)&sp10), arg1);
    guScaleF(((s32)&sp50), arg2, arg3, arg4);
    guMtxCatF(((s32)&sp50), ((s32)&sp10), ((s32)&sp90));
    guMtxF2L(((s32)&sp90), arg0);
}
