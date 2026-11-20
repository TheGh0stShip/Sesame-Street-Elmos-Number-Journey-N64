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

void __rmonSendData(char* const block, unsigned int blockSize) {
    int* blockPointer = (int*)block;
    unsigned int wordCount = (u32)(blockSize + 3) / 4;
    u32 data;
    union {
        char bufBytes[4];
        u32 bufWord;
    } buffer;

    if (((u32)block & 3) == 0) {
        while (wordCount--) {
            if ((u32)blockPointer >= SP_DMEM_START && (u32)blockPointer < 0x05000000) {
                __osSpRawReadIo((u32)blockPointer++, &data);
                __rmonIOputw(data);
            } else {
                __rmonIOputw(*(blockPointer++));
            }
        }
    } else
        while (wordCount--) {
            __rmonMemcpy((u8*)buffer.bufBytes, (u8*)blockPointer, sizeof(buffer));
            __rmonIOputw(buffer.bufWord);
            blockPointer++;
        }
    __rmonIOflush();
}
void rmonMain(void);

