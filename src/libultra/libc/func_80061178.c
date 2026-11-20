/* SDK libc/syncprintf.c */
#include "stdarg.h"
#include "PR/os.h"
#include "PR/os_internal.h"
#include "PR/rdb.h"
#include "xstdio.h"
#include "PR/rcp.h"

extern void* __printfunc;


extern u32 __kmc_pt_mode;
void* proutSyncPrintf(void* str, const char* buf, size_t n);
extern volatile unsigned int* stat;
extern volatile unsigned int* wport;
extern volatile unsigned int* piok;
void rmonPutchar(char c);
void* kmc_proutSyncPrintf(void* str, const char* buf, int n);

void osSyncPrintf(const char* fmt, ...) {
    int ans;
    va_list ap;

    va_start(ap, fmt);
    if (__kmc_pt_mode) {
        ans = _Printf(kmc_proutSyncPrintf, NULL, fmt, ap);
    } else {
        ans = _Printf(proutSyncPrintf, NULL, fmt, ap);
    }
    va_end(ap);
}
void rmonPrintf(const char* fmt, ...);

