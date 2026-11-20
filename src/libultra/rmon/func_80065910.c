#include "common.h"

typedef struct { u8 pad[4]; u8 unk_4; } P;
typedef struct { u8 pad[4]; u8 type; u8 pad5; s16 unk6; u8 pad8[4]; } Msg;
typedef s32 (*Handler)(P *);
extern Handler D_80078EB0[];
void __rmonSendReply(void *, s32, s32);
/* dispatch command by type; report errors */
s32 __rmonExecute(P *p) {
    s32 ret;
    Msg m;
    if (p->unk_4 >= 0x35) return -1;
    ret = D_80078EB0[p->unk_4](p);
    if (ret < 0) {
        m.type = p->unk_4;
        m.unk6 = ret;
        __rmonSendReply(&m, 0xC, 1);
    }
    return ret;
}
