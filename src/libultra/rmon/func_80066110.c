#include "common.h"

/* builds a 0x10-byte request on the stack and submits it */
typedef struct { u8 pad[4]; u8 unk_04; u8 pad2[7]; s32 unk_0C; } Src;
typedef struct { s32 unk_00; u8 unk_04; u8 pad; s16 unk_06; s32 unk_08; s32 unk_0C; } Req;
extern void __rmonSendReply(Req *, s32, s32);
s32 __rmonSetFault(Src *s)
{
    Req r;
    r.unk_04 = s->unk_04;
    r.unk_06 = 0;
    r.unk_0C = s->unk_0C;
    __rmonSendReply(&r, 0x10, 1);
    return 0;
}
