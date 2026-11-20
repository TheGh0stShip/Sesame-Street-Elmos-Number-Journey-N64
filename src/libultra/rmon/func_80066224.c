#include "common.h"

typedef struct { u8 pad[4]; u8 unk_4; } P;
typedef struct { u8 pad[4]; u8 type; u8 pad5; u16 unk6; u8 pad8[4]; s32 unkC; } Msg;
void __rmonSendReply(void *, s32, s32);
s32 __rmonSetComm(P *p) {
    Msg m;
    m.type = p->unk_4;
    m.unkC = 0;
    m.unk6 = 0;
    __rmonSendReply(&m, 0x10, 1);
    return 0;
}
