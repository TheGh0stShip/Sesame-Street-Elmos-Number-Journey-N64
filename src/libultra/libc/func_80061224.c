/* SDK libc/syncprintf.c */
#include "stdarg.h"
#include "PR/os.h"
#include "PR/os_internal.h"
#include "PR/rdb.h"
#include "xstdio.h"
#include "PR/rcp.h"

extern void* __printfunc;


extern u32 __kmc_pt_mode;

void* proutSyncPrintf(void* str, const char* buf, size_t n) {
    u32 sent = 0;

    while (sent < n) {
        sent += __osRdbSend(buf + sent, n - sent, RDB_TYPE_GtoH_PRINT);
    }
    return 1;
}
extern volatile unsigned int* stat;
extern volatile unsigned int* wport;
extern volatile unsigned int* piok;
void rmonPutchar(char c);
void* kmc_proutSyncPrintf(void* str, const char* buf, int n);
void osSyncPrintf(const char* fmt, ...);
void rmonPrintf(const char* fmt, ...);

