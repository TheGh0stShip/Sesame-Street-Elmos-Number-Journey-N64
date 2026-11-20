/* __rmonStopThread (libultra rmon/rmontask.c): stop a CPU thread or the RSP and reply with its status */
typedef struct OSThread_s {
    struct OSThread_s *next;
    s32 priority;
    struct OSThread_s *queue;
    struct OSThread_s *tlnext;
    u16 state;
    u16 flags;
    s32 id;
} OSThread;

typedef struct {
    s32 length;
    s8 code;
    s8 type;
    s16 error;
    s8 rev;
    u8 method;
    u8 unk_A[2];
} KKHeader;

typedef struct {
    KKHeader header;
    s32 object;
} KKObjectRequest;

typedef struct {
    KKHeader header;
    s32 flags;
    u8 unk_10[0x4C - 0x10];
} KKStatusEvent;

OSThread *func_80068350(void);                 /* __osGetActiveQueue */
void func_80068360(OSThread *);                /* osStopThread */
s32 func_800685A0(void);                       /* __rmonRCPrunning */
void func_800685C4(void);                      /* __rmonIdleRCP */
u32 func_800659E0(u32 *);                      /* __rmonReadWordAt */
s32 func_8006520C(s32, s32, void *);           /* __rmonGetBranchTarget */
void func_80068608(void);                      /* __rmonStepRCP */
s32 func_80066FD0(s32, s32, KKStatusEvent *);  /* __rmonGetThreadStatus */
void func_80060898(KKHeader *, s32, s32);      /* __rmonSendReply */

static int __rmonStopUserThreads(int whichThread) {
    register int whichOne = 0;
    register OSThread *tptr = func_80068350();

    if (whichThread != 0) {
        while (tptr->priority != -1) {
            if (tptr->id == whichThread) {
                break;
            }
            tptr = tptr->tlnext;
        }
        if (tptr->priority == -1) {
            return 0;
        }
        if (tptr->priority > 0 && tptr->priority <= 127) {
            func_80068360(tptr);
            whichOne = whichThread;
        }
    } else {
        while (tptr->priority != -1) {
            if (tptr->priority > 0 && tptr->priority <= 127) {
                func_80068360(tptr);
                whichOne = -1;
            }
            tptr = tptr->tlnext;
        }
    }
    return whichOne;
}

int func_80067200(KKHeader *req) {
    KKObjectRequest *request = (KKObjectRequest *)req;
    KKStatusEvent reply;
    u32 *pc;

    switch (req->method) {
        case 0:
            __rmonStopUserThreads(request->object);
            break;
        case 1:
            if (func_800685A0()) {
                func_800685C4();
                pc = (u32 *)func_800659E0((u32 *)0x04080000);
                if (pc == NULL) {
                    break;
                }
                pc--;
                if (func_8006520C(1, 1000, (void *)((u32)pc + 0x04001000)) % 4 == 0) {
                    func_80068608();
                }
            }
            break;
        default:
            return -4;
    }

    if (func_80066FD0(req->method, request->object, &reply) != 0) {
        return -2;
    }
    reply.header.code = request->header.code;
    reply.header.error = 0;
    func_80060898(&reply.header, sizeof(reply), 1);
    if (reply.flags == 1) {
        reply.header.code = 4;
        func_80060898(&reply.header, sizeof(reply), 2);
    }
    return 0;
}
