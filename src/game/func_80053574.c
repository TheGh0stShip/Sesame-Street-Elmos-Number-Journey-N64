#include "common.h"

void osPiStartDma(Mtx18 *, s32, s32, s32, s32, s32, Mtx18 *);
void osRecvMesg(Mtx18 *, s32, s32);
void osSendMesg(s32, s32, s32);

void func_80053574(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    Mtx18 a;
    Mtx18 b;
    Buf8 c;
    osCreateMesgQueue(&a, &c, 1);
    osInvalDCache(arg1, arg2);
    osPiStartDma(&b, 0, 0, arg0, arg1, arg2, &a);
    osRecvMesg(&a, 0, 1);
    osSendMesg(arg3, 0, 1);
}
