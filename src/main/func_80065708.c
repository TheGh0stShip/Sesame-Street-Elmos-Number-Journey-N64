/* rmon: report every stopped (break / CPU fault) application thread to the host */
typedef struct OSThread_s {
    struct OSThread_s *next; s32 priority; struct OSThread_s **queue; struct OSThread_s *tlnext;
    u16 state; u16 flags; s32 id; s32 fp;
    u8 pad[0x11C - 0x1C]; u32 *pc;
} OSThread;
typedef struct {
    s32 length; u8 code; u8 pad5; s16 unk_06; s32 unk_08; s32 unk_0C;
    s16 unk_10; s16 unk_12; s32 unk_14; s32 unk_18; s32 unk_1C; s32 unk_20;
    s16 unk_24; s16 unk_26; s32 unk_28; s32 unk_2C; s32 unk_30; s32 unk_34;
    s32 unk_38; s32 unk_3C; s32 unk_40; s32 unk_44; s32 unk_48;
} Stopped;
OSThread *func_80068350(void); /* __osGetActiveQueue */
void func_80066FD0(s32 cpu, s32 tid, Stopped *reply); /* __rmonGetThreadStatus */
void func_80060898(void *buf, s32 len, s32 kind); /* __rmonSendReply */
void func_80060BD0(OSThread *t); /* __rmonCopyWords? */

static void rmonSendBreakMessage(s32 whichThread, s32 breakNumber)
{
    Stopped reply;
    func_80066FD0(0, whichThread ? whichThread : 1003, &reply);
    reply.unk_0C = 1;
    reply.unk_10 = 2;
    reply.unk_12 = 0;
    reply.unk_30 = 0;
    reply.unk_24 = 2;
    reply.unk_26 = 4;
    reply.code = 4;
    reply.unk_06 = 0;
    reply.length = 0x4C;
    if (breakNumber == 15) {
        reply.unk_24 = 1;
        reply.unk_26 = 2;
    }
    if (breakNumber < 16) breakNumber = 0;
    else breakNumber -= 16;
    if (breakNumber) reply.unk_1C = 13;
    func_80060898(&reply, 0x4C, 2);
}

static void rmonSendFaultMessage(s32 whichThread)
{
    Stopped reply;
    func_80066FD0(0, whichThread ? whichThread : 1003, &reply);
    reply.unk_0C = 1;
    reply.unk_10 = 2;
    reply.unk_12 = 0;
    reply.unk_30 = 0;
    reply.unk_24 = 2;
    reply.unk_26 = 4;
    reply.code = 4;
    reply.unk_06 = 0;
    reply.length = 0x4C;
    reply.unk_24 = 1;
    reply.unk_26 = 2;
    func_80060898(&reply, 0x4C, 2);
}

void func_80065708(void)
{
    OSThread *tptr = func_80068350();
    u32 inst;

    while (tptr->priority != -1) {
        if (tptr->priority > 0 && tptr->priority <= 127) {
            if (tptr->flags & 1) {
                inst = *tptr->pc;
                if ((inst & 0xFC00003F) == 0xD) {
                    rmonSendBreakMessage(tptr->id, (s32)inst >> 6);
                } else {
                    rmonSendBreakMessage(tptr->id, 0);
                }
            }
            if (tptr->flags & 2) {
                func_80060BD0(tptr);
                rmonSendFaultMessage(tptr->id);
            }
        }
        tptr = tptr->tlnext;
    }
}
