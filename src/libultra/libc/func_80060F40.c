#include "stdarg.h"
#include "PR/os.h"
#include "PR/os_internal.h"
#include "PR/rdb.h"
#include "xstdio.h"
#include "PR/rcp.h"
/* not in the SDK headers */
extern volatile unsigned int *D_80077F98;
extern volatile unsigned int *D_80077F90;
extern volatile unsigned int *D_80077F94;
extern int D_80077F9C;


/* kmc_proutSyncPrintf: printf sink that expands tabs and sends length-prefixed lines to the KMC debugger port */
/* stat */
/* wport */
/* piok (PI_STATUS_REG) */
                    /* column */

static void rmonPutchar(u8 c) {
    while (*D_80077F98 & (1 | 2)) {
    }

    while (!(*D_80077F90 & 4)) {
    }

    *D_80077F94 = c;
}

void *kmc_proutSyncPrintf(void *str, const u8 *buf, int n) {
    int i;
    u8 c;
    u8 *p;
    u8 *q;
    u8 xbuf[128];

    p = xbuf;

    for (i = 0; i < n; i++) {
        c = *buf++;

        switch (c) {
            case '\n':
                *p++ = '\n';
                D_80077F9C = 0;
                break;
            case '\t':
                do {
                    *p++ = ' ';
                } while (++D_80077F9C % 8);
                break;
            default:
                D_80077F9C++;
                *p++ = c;
                break;
        }

        if (c == '\n' || (p - xbuf) > 100) {
            rmonPutchar((p - xbuf) - 1);

            q = xbuf;
            while (q != p) {
                rmonPutchar(*q++);
            }
            p = xbuf;
        }
    }
    if (p != xbuf) {
        rmonPutchar((p - xbuf) - 1);

        q = xbuf;
        while (q != p) {
            rmonPutchar(*q++);
        }
    }
    return (void *)1;
}
