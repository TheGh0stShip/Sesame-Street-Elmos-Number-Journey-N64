#include "common.h"

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
} KKStatusEvent;                 /* __osGetActiveQueue */
void osStopThread(OSThread *);/* osStopThread */
/* __rmonRCPrunning */
                      /* __rmonIdleRCP */
u32 __rmonReadWordAt(u32 *);/* __rmonReadWordAt */
/* __rmonGetBranchTarget */
                      /* __rmonStepRCP */
s32 __rmonGetThreadStatus(s32, s32, KKStatusEvent *);  /* __rmonGetThreadStatus */
void __rmonSendReply(KKHeader *, s32, s32);      /* __rmonSendReply */

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
            osStopThread(tptr);
            whichOne = whichThread;
        }
    } else {
        while (tptr->priority != -1) {
            if (tptr->priority > 0 && tptr->priority <= 127) {
                osStopThread(tptr);
                whichOne = -1;
            }
            tptr = tptr->tlnext;
        }
    }
    return whichOne;
}

int __rmonStopThread(KKHeader *req) {
    KKObjectRequest *request = (KKObjectRequest *)req;
    KKStatusEvent reply;
    u32 *pc;

    switch (req->method) {
        case 0:
            __rmonStopUserThreads(request->object);
            break;
        case 1:
            if (__rmonRCPrunning()) {
                __rmonIdleRCP();
                pc = (u32 *)__rmonReadWordAt((u32 *)0x04080000);
                if (pc == NULL) {
                    break;
                }
                pc--;
                if (__rmonGetBranchTarget(1, 1000, (void *)((u32)pc + 0x04001000)) % 4 == 0) {
                    __rmonStepRCP();
                }
            }
            break;
        default:
            return -4;
    }

    if (__rmonGetThreadStatus(req->method, request->object, &reply) != 0) {
        return -2;
    }
    reply.header.code = request->header.code;
    reply.header.error = 0;
    __rmonSendReply(&reply.header, sizeof(reply), 1);
    if (reply.flags == 1) {
        reply.header.code = 4;
        __rmonSendReply(&reply.header, sizeof(reply), 2);
    }
    return 0;
}
