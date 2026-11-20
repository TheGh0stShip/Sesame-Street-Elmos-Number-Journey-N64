/* SDK io/sptask.c */
#include "PR/os_internal.h"
#include "PR/ultraerror.h"
#include "PR/sptask.h"
#include "PR/rcp.h"
#include "PRinternal/osint.h"

#ident "$Revision: 1.4 $"

#define _osVirtualToPhysical(ptr)              \
    if (ptr != NULL) {                         \
        ptr = (void*)osVirtualToPhysical(ptr); \
    } (void)0

extern OSTask tmp_task;
OSTask* _VirtualToPhysicalTask(OSTask* intp);
void osSpTaskLoad(OSTask* intp);

void osSpTaskStartGo(OSTask* tp) {
    while (__osSpDeviceBusy()) {}

    __osSpSetStatus(SP_SET_INTR_BREAK | SP_CLR_SSTEP | SP_CLR_BROKE | SP_CLR_HALT);
}
