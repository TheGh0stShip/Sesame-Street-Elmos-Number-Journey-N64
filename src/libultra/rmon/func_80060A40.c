/* SDK rmon/rmonmain.c */
#include "PR/os_version.h"

#ident "$Revision: 3.70 $"
#ident "$Revision: 1.5 $"
#ident "$Revision: 1.2 $"
#ident "$Revision: 1.4 $"
#ident "$Revision: 1.3 $"


#include "PRinternal/dbgproto.h"
#include "PR/os_internal.h"
#include "PRinternal/rmonint.h"
#include "PR/rcp.h"
#include "PR/sptask.h"
#include "PR/rdb.h"

#include "PRinternal/macros.h"

extern // TODO: this comes from a header

int __rmonActive;
extern vu32 somethingToDo;
extern u32 inbuffer[280];
extern u8 cmdinptr;
extern u8 cmdoutptr;
extern int state;
extern char* inPointer;
void __rmonSendHeader(KKHeader* const block, u32 blockSize, u32 type);
void __rmonSendReply(KKHeader* const block, u32 blockSize, u32 replyType);
void __rmonSendData(char* const block, unsigned int blockSize);

void rmonMain(void) {
    register int newChars UNUSED;

    STUBBED_PRINTF(("rmon: Thread %d created\n"));
    STUBBED_PRINTF(("rmon: Thread %d destroyed\n"));

    somethingToDo = 0;
    cmdoutptr = 0;
    cmdinptr = 0;

    __rmonInit();
    __rmonActive = TRUE;


    state = 0, newChars = 0, inPointer = (void*)&inbuffer;
    for (;;) {
        OSMesg work;

        osRecvMesg(&__rmonMQ, &work, OS_MESG_BLOCK);

        somethingToDo |= (u32)work;

        if (somethingToDo & RMON_MESG_CPU_BREAK) {
            somethingToDo &= ~RMON_MESG_CPU_BREAK;
            __rmonHitBreak();
        }
        if (somethingToDo & RMON_MESG_SP_BREAK) {
            somethingToDo &= ~RMON_MESG_SP_BREAK;
            __rmonHitSpBreak();
        }
        if (somethingToDo & RMON_MESG_FAULT) {
            somethingToDo &= ~RMON_MESG_FAULT;
            __rmonHitCpuFault();
        }
        if (somethingToDo & 0x10) {
            somethingToDo;
            somethingToDo &= (u8)~0x10;
        }
        if (somethingToDo & 0x20) {
            somethingToDo;
            somethingToDo &= (u8)~0x20;
        }
    }
}

