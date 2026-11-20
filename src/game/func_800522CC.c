#include "common.h"
 /* first used */
extern DmaBuf *D_800A05A4;/* first free */
/* buffer length */
/* next message index */
    /* message queue */

s32 osEPiStartDma(void *handle, IoMesg *mb, s32 direction);

/* audio DMA: finds a cached buffer covering [addr, addr+len) or starts a PI DMA into a free one */
DmaBuf *func_800522CC(u32 addr, s32 len) {
    void *handle;
    DmaBuf *dma;
    DmaBuf *it;
    DmaBuf *nx;
    DmaBuf *last;
    IoMesg *m;
    s32 n;
    u32 end;

    if ((addr & 0xFF000000) == 0xFF000000) {
        handle = D_800BD854;
        addr &= 0xFFFFFF;
        addr += 0x140000;
    } else {
        if (D_8012C408 & 1) {
            return NULL;
        }
        handle = D_800A05D8;
    }
    last = NULL;
    end = addr + len;
    for (it = D_800A05A0; it != NULL; it = it->next) {
        if (addr < it->startAddr) {
            break;
        }
        if (end <= it->startAddr + D_800A05B4) {
            return it;
        }
        last = it;
    }
    dma = D_800A05A4;
    if (dma == NULL) {
        return D_800A05A0;
    }
    D_800A05A4 = dma->next;
    if (last != NULL) {
        nx = last->next;
        dma->next = nx;
        if (nx != NULL) {
            nx->prev = dma;
        }
        dma->prev = last;
        last->next = dma;
    } else {
        dma->prev = NULL;
        dma->next = D_800A05A0;
        if (D_800A05A0 != NULL) {
            D_800A05A0->prev = dma;
        }
        D_800A05A0 = dma;
    }
    n = D_800A05B8;
    dma->startAddr = addr & ~1;
    dma->lastFrame = 2;
    D_800A05B8 = n + 1;
    m = &D_800A05AC[n];
    m->pri = 0;
    m->retQueue = D_800A05C0;
    m->dramAddr = dma->ptr;
    m->devAddr = dma->startAddr;
    m->size = D_800A05B4;
    osEPiStartDma(handle, m, 0);
    return dma;
}
