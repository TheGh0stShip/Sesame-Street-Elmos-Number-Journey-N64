/* libultra rmon __rmonGetThreadStatus: fill a status reply for a CPU thread or the RSP */
typedef struct {
    s32 length;
    u8 code;
    u8 error;
    s16 unk_06;
    s32 method;
} KKHeader;
typedef struct {
    s32 flags;
    s16 why;
    s16 what;
    s32 tid;
    s32 pid;
    u32 instr;
    struct {
        u32 addr;
        s16 major;
        s16 minor;
    } info;
    s32 priority;
    s32 start;
    s32 rv;
} KKThreadStatus;
typedef struct {
    KKHeader header;
    KKThreadStatus status;
} KKStatusEvent;
typedef struct OSThread_s {
    struct OSThread_s *next;
    s32 priority;
    struct OSThread_s **queue;
    struct OSThread_s *tlnext;
    u16 state;
    u16 flags;
    s32 id;
    s32 fp;
    u8 unk_1C[0x100];
    u32 pc; /* context.pc */
} OSThread;

extern u8 D_8013EC80; /* __rmonRcpAtBreak */
extern s32 func_800685A0(void); /* __rmonRCPrunning */
extern u32 func_800659E0(u32 *addr); /* __rmonReadWordAt */
extern OSThread *func_80068350(void); /* __osGetActiveQueue */

s32 func_80066FD0(s32 method, s32 id, KKStatusEvent *reply) {
    u32 inst;

    reply->status.tid = id;
    reply->status.pid = (method == 1) ? 1001 : 1002;
    reply->status.why = 1;
    reply->status.what = 0;
    reply->status.info.major = 0;
    reply->status.info.minor = 0;
    reply->status.rv = 0;

    if (method == 1) {
        reply->status.start = 0x04001000;
        reply->status.priority = 42;
        if (func_800685A0()) {
            reply->status.flags = 4;
            reply->status.info.addr = 0;
            reply->status.instr = 0;
        } else {
            reply->status.flags = 1;
            reply->status.info.addr = func_800659E0((u32 *)0x04080000) + 0x04001000;
            inst = func_800659E0((u32 *)reply->status.info.addr);
            if ((inst & 0xFC00003F) == 0xD) {
                inst = 0xD;
            }
            if (D_8013EC80) {
                reply->status.why = 2;
                reply->status.info.major = 2;
                reply->status.info.minor = 4;
            }
            reply->status.instr = inst;
        }
    } else {
        OSThread *tptr = func_80068350();

        while (tptr->priority != -1) {
            if (tptr->id == id) {
                break;
            }
            tptr = tptr->tlnext;
        }
        if (tptr->priority == -1) {
            return -2;
        }
        reply->status.priority = tptr->priority;
        reply->status.flags = (tptr->state != 0) ? tptr->state : 1;
        reply->status.info.addr = tptr->pc;
        inst = *(u32 *)(tptr->pc);
        if ((inst & 0xFC00003F) == 0xD) {
            inst = 0xD;
        }
        reply->status.instr = inst;
        reply->status.start = (s32)tptr;
        if (tptr->flags & 1) {
            reply->status.why = 2;
            reply->status.info.major = 2;
            reply->status.info.minor = 4;
        } else if (tptr->flags & 2) {
            reply->status.why = 2;
            reply->status.info.major = 1;
            reply->status.info.minor = 2;
        }
    }
    return 0;
}
