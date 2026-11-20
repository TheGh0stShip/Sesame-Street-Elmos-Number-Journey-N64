#include "PR/os_internal.h"
#include "PRinternal/dbgproto.h"
#include "PR/rcp.h"
#include "PR/sptask.h"
#include "PRinternal/rmonint.h"
#include "PRinternal/macros.h"


typedef struct {
    s32 size;      /* 0x00 */
    u8 type;       /* 0x04 */
    u8 pad5;
    u16 unk_06;
    u8 pad8[4];
    s32 unk_0C;
    u16 unk_10;
    u16 unk_12;
    u8 pad14[0x10];
    u16 unk_24;
    u16 unk_26;
    u8 pad28[8];
    s32 unk_30;
    u8 pad34[0x18];
} Msg;
void __rmonHitSpBreak(void) {
    Msg m;
    __rmonWriteWordTo(0x04080000, __rmonReadWordAt(0x04080000) - 4);
    __rmonGetThreadStatus(1, 1000, &m);
    m.unk_0C = 1;
    m.unk_10 = 2;
    m.unk_24 = 2;
    m.unk_26 = 4;
    m.type = 4;
    m.unk_12 = 0;
    m.unk_30 = 0;
    m.unk_06 = 0;
    m.size = 0x4C;
    __rmonSendReply(&m, 0x4C, 2);
    __rmonRcpAtBreak = 1;
}
